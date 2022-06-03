#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1

// list node
typedef struct Node {
	int *data;
	int array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
	int size;
} List;

// iterator
typedef struct iterator {
	int position;
	struct Node* node_ptr;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = {0, head};
	return it;
}

// backward initialization
iterator end(Node* tail) {
	iterator it = {tail->array_size - 1, tail};
	return it;
}

// initialize list
void init(List *list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

// ---------------------------------------------- to implement ...

// append element to the list
void push_back(List *list, int *data, int array_size) {
	Node *new = malloc(sizeof(Node));
	new->array_size=array_size;
	new->data=data;
	new->prev=NULL;
	new->next=NULL;
	if (!list->head)
	{
		list->head=new;
		list->tail=list->head;
		list->size+=array_size;
	}
	else
	{
		Node *ptr = (Node*)list->head;
		while (ptr->next)
		{
			ptr=ptr->next;
		}
		ptr->next=new;
		new->prev=ptr;
		list->tail=new;
		list->size+=array_size;
	}
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, int n) {
	Node *ptr = (Node*)itr->node_ptr;
	int d=0;
	while (ptr->array_size+d<n)
	{
		d+=ptr->array_size;
		ptr=ptr->next;
	}
	itr->node_ptr=ptr;
	itr->position=n-d;
}

// forward iteration - get n-th element in the list
int get_forward(List *list, int n) {
	iterator *i = malloc(sizeof(iterator));
	i->node_ptr=list->head;
	skip_forward(i, n);
	int p = ((int*)((Node*)i->node_ptr)->data)[i->position-1];
	//free(i);
	return p;
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, int n) {
	Node *ptr = (Node*)itr->node_ptr;
	int d=0;
	while (ptr->array_size+d<n)
	{
		d+=ptr->array_size;
		ptr=ptr->prev;
	}
	itr->node_ptr=ptr;
	itr->position=ptr->array_size-(n-d);
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, int n) {
	iterator *i = malloc(sizeof(iterator));
	i->node_ptr=list->tail;
	skip_backward(i, n);
	int p = ((int*)((Node*)i->node_ptr)->data)[i->position];
	free(i);
	return p;
}

// remove n-th element; if array empty remove node
void remove_at(List *list, int n) {
	iterator *i = malloc(sizeof(iterator));
	i->node_ptr=list->head;
	skip_forward(i, n);
	for (int j = i->position-1; j < i->node_ptr->array_size-1; j++)
	{
		i->node_ptr->data[j]=i->node_ptr->data[j+1];
	}
	i->node_ptr->array_size--;
	if (i->node_ptr->array_size==0)
	{
		if (list->head==i->node_ptr)
		{
			list->head=i->node_ptr->next;
			free(i->node_ptr);
		}
		else if (list->tail==i->node_ptr)
		{
			Node *ptr = (Node*) i->node_ptr->prev;
			ptr->next=NULL;
			list->tail=ptr;
			free(i->node_ptr);
		}
		else
		{
			Node *l = (Node*) i->node_ptr->prev;
			Node *r=(Node*)i->node_ptr->next;
			l->next=r;
			r->prev=l;
			free(i->node_ptr);
		}
		list->size--;
	}
}

// -------------------- helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head; node != NULL; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// remove the first element
void delete_front(List *list) {
	Node *to_delete;
	if(list->head == NULL) return;
	to_delete = list->head;
	list->head = list->head->next;
	if(list->head == NULL) list->tail = NULL;
	free(to_delete->data);
	free(to_delete);
	list->size--;
}

// free list
void freeList(List *list) {
	while(list->head) {
		delete_front(list);
	}
}

// read int vector
void read_vector(int tab[], int n) {
	for (int i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int size, n;
	init(list);
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &size);
		int *tab = (int*) malloc((size_t)size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, size, m;
	List list;

	scanf("%d", &to_do);
	read_list(&list);
	switch (to_do) {
	case 1:
		dumpList(&list);
		break;
	case 2:
		scanf("%d", &size);
		for (int i = 0; i < size; i++) {
			scanf("%d", &m);
			printf("%d ", get_forward(&list, m));
		}
		printf("\n");
		break;
	case 3:
		scanf("%d", &size);
		for (int i = 0; i < size; i++) {
			scanf("%d", &m);
			printf("%d ", get_backward(&list, m));
		}
		printf("\n");
		break;
	case 4:
		scanf("%d", &size);
		for (int i=0; i < size; i++) {
			scanf("%d", &m);
			remove_at(&list, m);
		}
		dumpList(&list);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	freeList(&list);

	return 0;
}
