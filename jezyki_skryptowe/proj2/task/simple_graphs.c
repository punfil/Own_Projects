#include <stdlib.h>

#include <python3.11/Python.h>
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
    /*
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
    */
    return 0;
}

static PyMemberDef AdjacencyMatrix_members[] = {
    {"vertices", T_USHORT, offsetof(AdjacencyMatrix, vertices), 0, NULL},
    {"edges", T_PYSSIZET, offsetof(AdjacencyMatrix, edges), 0, NULL},
    {NULL}  // Sentinel
};

static PyMethodDef AdjacencyMatrix_methods[] = {
    {NULL, NULL, 0, NULL}  // Empty for now
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
    if (mod == NULL){
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