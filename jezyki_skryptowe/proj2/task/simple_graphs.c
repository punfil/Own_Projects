#include <stdlib.h>

#include <Python.h>
#include "structmember.h"
#include <stdio.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_VERTICES 16

typedef struct {
    PyObject_HEAD
    unsigned short vertices;
    unsigned short *edges;
} AdjacencyMatrix;

static PyObject *AdjacencyMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AdjacencyMatrix *self = (AdjacencyMatrix *) type->tp_alloc(type, 0);
    if (!self) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for edges");
        return NULL;
    }

    self->vertices = 0x0000;
    self->edges = (unsigned short *)calloc(MAX_VERTICES, sizeof(unsigned short));
    if (self->edges == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for edges");
        return NULL;
    }

    PyObject_INIT(self, type);

    return (PyObject *)self;
}

static void AdjacencyMatrix_dealloc(AdjacencyMatrix *self) {
    free(self->edges);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int AdjacencyMatrix_init(AdjacencyMatrix *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"text", NULL};
    char *txt = "?";

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|s", keywords, &txt)) {
        return -1;
    }

    int vertices_count = 0;
    for (int i = 0; i < txt[0] - 63; i++) {
        self->vertices = self->vertices << 1;
        self->vertices = self->vertices | 0x0001;
        vertices_count++;
    }

    int k = 0;
    int i = 1;
    int c = 0;

    for (int v = 1; v < vertices_count; v++) {
        for (int u = 0; u < v; u++) {
            if (!k) {
                c = txt[i] - 63;
                i++;
                k = 6;
            }

            k--;
            if ((c & (1 << k)) != 0) {
                self->edges[u] = self->edges[u] | (0x0001 << v);
                self->edges[v] = self->edges[v] | (0x0001 << u);
            }
        }
    }

    return 0;
}

static PyObject *number_of_vertices(AdjacencyMatrix *self) {
    long ctr = 0;
    unsigned short vertices = self->vertices;
    for (int i = 0; i < MAX_VERTICES; i++) {
        if ((vertices & 1)) {
            ctr++;
        }

        vertices = vertices >> 1;
    }

    return PyLong_FromLong(ctr);
}


static PyObject *vertices(AdjacencyMatrix *self) {
    PyObject *vertices_set = PySet_New(NULL);
    if (!vertices_set) {
        return NULL;
    }

    for (int i = 0; i < 16; i++) {
        if (((self->vertices >> i) & 0x0001) == 1) {
            PyObject *item = PyLong_FromLong(i);
            PySet_Add(vertices_set, item);
            Py_DECREF(item);
        }
    }

    return vertices_set;
}

static PyObject *vertex_degree(AdjacencyMatrix *self, PyObject *args) {
    int v;

    if (args != NULL) {
        PyArg_ParseTuple(args, "i", &v);
    }

    short ctr = 0;
    short edges = self->edges[v];
    for (int i = 0; i < 16; i++) {
        if ((edges & 0x0001) == 1) {
            ctr++;
        }
        edges = edges >> 1;
    }

    long ret = (long) ctr;
    return PyLong_FromLong(ret);
}

static PyObject *vertex_neighbors(AdjacencyMatrix *self, PyObject *args) {
    int v;

    if (args != NULL) {
        PyArg_ParseTuple(args, "i", &v);
    }
    PyObject *neighbors_set = PySet_New(NULL);

    short edges = self->edges[v];
    for (int i = 0; i < 16; i++) {
        if ((edges & 0x0001) == 1) {
            PyObject *item = PyLong_FromLong(i);
            PySet_Add(neighbors_set, item);
            Py_DECREF(item);
        }
        edges = edges >> 1;
    }

    return neighbors_set;
}

static PyObject *add_vertex(AdjacencyMatrix *self, PyObject *args) {
    int v;

    if (args != NULL) {
        PyArg_ParseTuple(args, "i", &v);
    }

    short tmp = (0x0001 << v);
    self->vertices = self->vertices | tmp;
    return PyBool_FromLong(1);
}

static PyObject *delete_vertex(AdjacencyMatrix *self, PyObject *args) {
    int v;

    if (args != NULL) {
        PyArg_ParseTuple(args, "i", &v);
    }

    self->edges[v] = 0x0000;

    short tmp = 0x0001 << v;
    tmp = ~tmp;

    for (int i = 0; i < 16; i++) {
        self->edges[i] = self->edges[i] & tmp;
    }
    self->vertices = self->vertices & tmp;
    return PyBool_FromLong(1);
}

static PyObject *number_of_edges(AdjacencyMatrix *self) {
    short ctr = 0;
    for (int j = 0; j < 16; j++) {
        short edges = self->edges[j];
        for (int i = 0; i < 16; i++) {
            if ((edges & 1) == 1) {
                ctr++;
            }
            edges = edges >> 1;
        }
    }
    long ret = ctr / 2;
    return PyLong_FromLong(ret);
}

static PyObject *edges(AdjacencyMatrix *self) {
    PyObject *edges_set = PySet_New(NULL);
    if (!edges_set) {
        return NULL;
    }

    for (int j = 0; j < 16; j++) {
        short edges = self->edges[j];
        for (int i = 0; i < 16; i++) {
            if ((edges & 0x0001) == 1) {
                PyObject *edge = PyTuple_New(2);
                PyTuple_SetItem(edge, 0, PyLong_FromLong(MIN(i, j)));
                PyTuple_SetItem(edge, 1, PyLong_FromLong(MAX(i, j)));
                PySet_Add(edges_set, edge);
                Py_DECREF(edge);
            }
            edges = edges >> 1;
        }
    }

    return edges_set;
}

static PyObject *is_edge(AdjacencyMatrix *self, PyObject *args) {
    int v, u;

    if (args != NULL) {
        PyArg_ParseTuple(args, "ii", &v, &u);
    }

    short edges_v = self->edges[v];
    edges_v = edges_v >> u;
    edges_v = edges_v & 0x0001;

    return PyBool_FromLong(edges_v);
}

static PyObject *add_edge(AdjacencyMatrix *self, PyObject *args) {
    int v, u;

    if (args != NULL) {
        PyArg_ParseTuple(args, "ii", &v, &u);
    }

    if (v != u) {
        short update_u = (1 << v);
        short update_v = (1 << u);
        self->edges[v] = self->edges[v] | update_v;
        self->edges[u] = self->edges[u] | update_u;
    }
    return PyBool_FromLong(1);
}

static PyObject *delete_edge(AdjacencyMatrix *self, PyObject *args) {
    int v, u;

    if (args != NULL) {
        PyArg_ParseTuple(args, "ii", &v, &u);
    }

    short update_u = ~(0x0001 << v);
    short update_v = ~(0x0001 << u);
    self->edges[v] = self->edges[v] & update_v;
    self->edges[u] = self->edges[u] & update_u;
    return PyBool_FromLong(1);
}

static PyMemberDef AdjacencyMatrix_members[] = {
    {"vertices", T_USHORT, offsetof(AdjacencyMatrix, vertices), 0, NULL},
    {"edges", T_PYSSIZET, offsetof(AdjacencyMatrix, edges), 0, NULL},
    {NULL}  // Sentinel
};

static PyMethodDef AdjacencyMatrix_methods[] = {
    {"number_of_vertices", (PyCFunction) number_of_vertices, METH_NOARGS, NULL},
    {"vertices",           (PyCFunction) vertices,           METH_NOARGS, NULL},
    {"vertex_degree",      (PyCFunction) vertex_degree,      METH_VARARGS, NULL},
    {"vertex_neighbors",   (PyCFunction) vertex_neighbors,   METH_VARARGS, NULL},
    {"add_vertex",         (PyCFunction) add_vertex,         METH_VARARGS, NULL},
    {"delete_vertex",      (PyCFunction) delete_vertex,      METH_VARARGS, NULL},
    {"number_of_edges",    (PyCFunction) number_of_edges,    METH_NOARGS, NULL},
    {"is_edge",            (PyCFunction) is_edge,            METH_VARARGS, NULL},
    {"add_edge",           (PyCFunction) add_edge,           METH_VARARGS, NULL},
    {"delete_edge",        (PyCFunction) delete_edge,        METH_VARARGS, NULL},
    {"edges",              (PyCFunction) edges,              METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyTypeObject AdjacencyMatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_simple_graphs.AdjacencyMatrix",
    .tp_basicsize = sizeof(AdjacencyMatrix),
    .tp_new = AdjacencyMatrix_new,
    .tp_dealloc = (destructor)AdjacencyMatrix_dealloc,
    .tp_init = (initproc)AdjacencyMatrix_init,
    .tp_members = AdjacencyMatrix_members,
    .tp_methods = AdjacencyMatrix_methods  // Point to methods array
};

static struct PyModuleDef simple_graph_module = {
    PyModuleDef_HEAD_INIT,
    "_simple_graphs",
    NULL,
    -1
};

PyMODINIT_FUNC PyInit_simple_graphs(void) {
    PyObject *mod = PyModule_Create(&simple_graph_module);
    if (mod == NULL) {
        PyErr_SetString(PyExc_ModuleNotFoundError, "Failed to create the module");
        return NULL;
    }

    if (PyType_Ready(&AdjacencyMatrixType) < 0) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize AdjacencyMatrix type");
        Py_DECREF(mod);
        return NULL;
    }

    Py_INCREF(&AdjacencyMatrixType);
    if (PyModule_AddObject(mod, "AdjacencyMatrix", (PyObject *)&AdjacencyMatrixType) < 0) {
        Py_DECREF(&AdjacencyMatrixType);
        Py_DECREF(mod);
        return NULL;
    }

    return mod;
}
