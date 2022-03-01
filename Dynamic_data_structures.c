#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//Singly linked list
typedef struct singly_linked {
	int value;
	struct singly_linked* next;
}singly_linked;


int add_singly_linked(singly_linked** head, int value) {
	singly_linked* new_node = (singly_linked*)malloc(sizeof(singly_linked));
	if (new_node == NULL) { return 0; } //No memory has been allocated -> return false
	new_node->value = value;
	new_node->next = NULL;
	if (*head == NULL) {
		*head = new_node;
		return 1;
	}
	singly_linked* copy = *head;
	while (copy->next != NULL) {
		copy = copy->next;
	}
	copy->next = new_node;
	return 1;
}


int remove_singly_linked(singly_linked** head, int value) {
	if (*head == NULL) {
		return 0;
	}
	else if ((*head)->next == NULL) {
		free(*head);
		*head = NULL;
		return 1;
	}
	singly_linked* copy = *head;
	while (copy->next->next != NULL) {
		copy = copy->next;
	}
	free(copy->next);
	copy->next = NULL;
	return 1;
}


void print_singly_linked(singly_linked* head) {
	singly_linked* copy = head;
	while (copy != NULL) {
		printf("%d ", copy->value);
		copy = copy->next;
	}
	printf("\n");
}


//Doubly Linked List
typedef struct doubly_linked {
	struct doubly_linked* previous;
	int value;
	struct doubly_linked* next;
}doubly_linked;


int add_doubly_linked(doubly_linked** head, int value) {
	doubly_linked* new_node = (doubly_linked*)malloc(sizeof(doubly_linked));
	if (new_node == NULL) { return 0; }
	new_node->value = value;
	new_node->next = NULL;
	if (*head == NULL) {
		new_node->previous = NULL;
		*head = new_node;
		return 1;
	}
	doubly_linked* copy = *head;
	while (copy->next != NULL) {
		copy = copy->next;
	}
	new_node->previous = copy;
	copy->next = new_node;
	return 1;
}


int remove_doubly_linked(doubly_linked** head, int value) {
	if (*head == NULL) {
		return 1;
	}
	if ((*head)->next == NULL) {
		free(*head);
		*head = NULL;
		return 1;
	}
	doubly_linked* copy = *head;
	while (copy->next->next != NULL) {
		copy = copy->next;
	}
	free(copy->next);
	copy->next = NULL;
	return 1;
}


void print_doubly_linked(doubly_linked* head) {
	doubly_linked* copy = head;
	while (copy != NULL) {
		printf("%d ", copy->value);
		copy = copy->next;
	}
	printf("\n");
}

//Kolejka na liscie jednokierunkowej
//Struct na jednokierunkowa
int enqueue(singly_linked** head, singly_linked** tail, int value) {
	singly_linked* new_element = (singly_linked*)malloc(sizeof(singly_linked));
	if (new_element == NULL) { return 0; }
	new_element->value = value;
	if (*head == *tail && *head == NULL){
		*head = new_element;
		*tail = new_element;
		new_element->next = NULL;
		return 1;
	}
	(*tail)->next = new_element;
	new_element->next = NULL;
	*tail = new_element;
	return 1;
}

int dequeue(singly_linked** head, singly_linked** tail) {
	if (*head == *tail) {
		if (*head == NULL) {
			return 0;
		}
		free(*head);
		*head = NULL;
		*tail = NULL;
		return 1;
	}
	singly_linked* temp = (*head)->next;
	free(*head);
	*head = temp;
	return 1;
}


int add_in_order_doubly_linked(doubly_linked** head, int value) {
	doubly_linked* new_element = (doubly_linked*)malloc(sizeof(doubly_linked));
	if (new_element == NULL) { return 0; }
	new_element->value = value;
	if (*head == NULL) {
		*head = new_element;
		new_element->next = NULL;
		new_element->previous = NULL;
		return 1;
	}
	doubly_linked* copy = *head;
	while (copy->next != NULL && copy->next->value < value) {
		copy = copy->next;
	}
	new_element->next = copy->next;
	new_element->previous = copy;
	copy->next = new_element;
	copy->next->previous = new_element;
		
	return 1;
}


typedef struct tree {
	int value;
	struct tree* left;
	struct tree* right;
}tree;

int add_tree(tree** head, int value) {
	tree* new_element = (tree*)malloc(sizeof(tree));
	if (new_element == NULL) { return 0; }
	new_element->value = value;
	new_element->left = NULL;
	new_element->right = NULL;
	if (*head == NULL) {
		*head = new_element;
		return 1;
	}
	tree* copy = *head;
	while (copy != NULL) {
		if (copy->value >= value) {
			if (copy->left == NULL) {
				copy->left = new_element;
				return 1;
			}
			copy = copy->left;
		}
		else {
			if (copy->right == NULL) {
				copy->right = new_element;
				return 1;
			}
			copy = copy->right;
		}
	}
	return 1;
}


int add_tree_recursive(tree** head, int value) {
	if (*head == NULL) {
		*head = (tree*)malloc(sizeof(tree));
		if (*head == NULL) { return 0; }
		(*head)->value = value;
		(*head)->left = NULL;
		(*head)->right = NULL;
		return 1;
	}
	if ((*head)->value >= value) {
		if ((*head)->left == NULL) {
			(*head)->left = (tree*)malloc(sizeof(tree));
			(*head)->left->value = value;
			(*head)->left->left = NULL;
			(*head)->left->right = NULL;
			return 1;
		}
		return add_tree_recursive(&((*head)->left), value);
	}
	else {
		if ((*head)->right == NULL) {
			(*head)->right = (tree*)malloc(sizeof(tree));
			(*head)->right->value = value;
			(*head)->right->left = NULL;
			(*head)->right->right = NULL;
			return 1;
		}
		return add_tree_recursive(&((*head)->right), value);
	}
	
}

void print_tree(tree* head) {
	if (head->left != NULL) {
		print_tree(head->left);
	}
	printf(" %d ", head->value);
	if (head->right != NULL) {
		print_tree(head->right);
	}
}


int main() {
	singly_linked* my_singly_linked = NULL;
	doubly_linked* my_double_linked = NULL;
	singly_linked* queue_head = NULL;
	singly_linked* queue_tail = NULL;
	tree* my_tree = NULL;
	printf("Work of singly linked list\n");
	for (int i = 0; i < 5; i++) {
		add_singly_linked(&my_singly_linked, i);
		print_singly_linked(my_singly_linked);
	}
	for (int i = 0; i < 5; i++) {
		remove_singly_linked(&my_singly_linked, i);
		print_singly_linked(my_singly_linked);
	}
	for (int i = 0; i < 5; i++) {
		add_singly_linked(&my_singly_linked, i);
		print_singly_linked(my_singly_linked);
	}
	for (int i = 0; i < 5; i++) {
		remove_singly_linked(&my_singly_linked, i);
		print_singly_linked(my_singly_linked);
	}
	printf("Work of doubly linked list\n");
	for (int i = 0; i < 5; i++) {
		add_doubly_linked(&my_double_linked, i);
		print_doubly_linked(my_double_linked);
	}
	for (int i = 0; i < 5; i++) {
		remove_doubly_linked(&my_double_linked, i);
		print_doubly_linked(my_double_linked);
	}
	for (int i = 0; i < 5; i++) {
		add_doubly_linked(&my_double_linked, i);
		print_doubly_linked(my_double_linked);
	}
	for (int i = 0; i < 5; i++) {
		remove_doubly_linked(&my_double_linked, i);
		print_doubly_linked(my_double_linked);
	}
	printf("Work of queue on singly linked list\n");
	for (int i = 0; i < 5; i++) {
		enqueue(&queue_head, &queue_tail, i);
		print_singly_linked(queue_head);
	}
	for (int i = 0; i < 5; i++) {
		dequeue(&queue_head, &queue_tail);
		print_singly_linked(queue_head);
	}
	for (int i = 0; i < 5; i++) {
		enqueue(&queue_head, &queue_tail, i);
		print_singly_linked(queue_head);
	}
	for (int i = 0; i < 5; i++) {
		dequeue(&queue_head, &queue_tail);
		print_singly_linked(queue_head);
	}
	printf("Parametrized doubly linked list - parameteres are inserted in order \n");
	for (int i = 0; i < 5; i++) {
		add_in_order_doubly_linked(&my_double_linked, rand());
		print_doubly_linked(my_double_linked);
	}
	for (int i = 0; i < 5; i++) {
		remove_doubly_linked(&my_double_linked, i);
		print_doubly_linked(my_double_linked);
	}
	printf("Tree - elements are printed and added in order\n");
	for (int i = 0; i < 5; i++) {
		add_tree_recursive(&my_tree, i+100);
		print_tree(my_tree);
		printf("\n");
	}
	for (int i = 0; i < 5; i++) {
		add_tree_recursive(&my_tree, i);
		print_tree(my_tree);
		printf("\n");
	}
	return 0;
}