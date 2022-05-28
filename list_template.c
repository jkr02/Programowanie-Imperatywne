#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;
typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef void (*InsertInOrder)(struct tagList*, void*);

typedef struct tagListElement {
	struct tagListElement *next;
	void                  *data;
} ListElement;

typedef struct tagList {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp      free_data;
	CompareDataFp compare_data;
	InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data, CompareDataFp compare_data, InsertInOrder insert_sorted) {
	p_list->dump_data=dump_data;
	p_list->free_data=free_data;
	p_list->compare_data=compare_data;
	p_list->insert_sorted=insert_sorted;
	p_list->head=NULL;
	p_list->tail=NULL;
}

// Print all elements of the list
void dump_list(const List* p_list) {
	ListElement *ptr=(ListElement*)p_list->head;
	while (ptr!=NULL)
	{
		p_list->dump_data(ptr);
		ptr=ptr->next;
	}	
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
	ListElement *ptr = p_list->head;
	while (ptr!=NULL)
	{
		if (p_list->compare_data(ptr->data, data))
		{
			p_list->dump_data(ptr);
		}
		ptr=ptr->next;
	}
}

// Free all elements of the list
void free_list(List* p_list) {
	ListElement *ptr1=p_list->head;
	ListElement *ptr2;
	while (ptr1)
	{
		ptr2=ptr1->next;
		p_list->free_data(ptr1);
		ptr1=ptr2;
	}
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
	ListElement *ptr=malloc(sizeof(ListElement));
	if(ptr == NULL) exit(MEMORY_ALLOCATION_ERROR);
	if (!p_list->head)
	{
		ptr->data=data;
		ptr->next=p_list->head;
		p_list->head=ptr;
		p_list->tail=ptr;
	}
	else
	{
		ptr->data=data;
		ptr->next=p_list->head;
		p_list->head=ptr;
	}
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element->data = data;
	element->next = NULL;
	if(p_list->tail) p_list->tail->next = element;
	p_list->tail = element;
	if(!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {
	if (p_list->head)
	{
		ListElement *ptr=(ListElement*)p_list->head;
		p_list->head=ptr->next;
		if (p_list->head=NULL)
		{
			p_list->tail=NULL;
		}
		p_list->free_data(ptr);
	}
}

// Reverse the list
void reverse(List *p_list) {
	if (p_list->head)
	{
		ListElement *ptr1=p_list->head, *ptr2, *ptr_first=ptr1;
		while (p_list->head!=p_list->tail)
		{
			ptr1=p_list->head;
			p_list->head=p_list->head->next;
			ptr1->next=p_list->tail->next;
			p_list->tail->next=ptr1;
		}
		p_list->tail=ptr_first;
	}
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
	if (!p_list->head)
	{
		push_back(p_list, data);
	}
	else
	{
		// ListElement *ptr = (ListElement*)(p_list->head);
		// int in_list=0;
		// while (ptr)
		// {
		// 	if (p_list->compare_data(ptr->data, data)==0)
		// 	{
		// 		in_list=1;
		// 		break;
		// 	}
		// 	ptr=ptr->next;
		// }
		// if (in_list==0)
		// {
			p_list->insert_sorted(p_list, data);
		//}
	}
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
	ListElement *insertionpoint=NULL;
	ListElement *current=NULL;
	for (current = p_list->head; current!=NULL; current=current->next){
		if (p_list->compare_data(current, p_element)<=0)
		{
			insertionpoint=current;
		}
	}
	return insertionpoint;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
	ListElement *ptr=malloc(sizeof(ListElement));
	ptr->data=data;
	ptr->next=NULL;
	if (p_list->tail==previous)
	{
		previous->next=ptr;
		p_list->tail=ptr;
	}
	else
	{
		ListElement *ptr1=previous->next;
		ptr->next=ptr1;
		previous->next=ptr;
	}
	
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data) {
	if (!p_list->head)
	{
		push_back(p_list, p_data);
	}
	else
	{
		ListElement *new=malloc(sizeof(ListElement));
		new->data=p_data;
		new->next=NULL;
		ListElement *insertionpoint=(ListElement*)find_insertion_point(p_list, new);
		if (insertionpoint==NULL)
		{
			new->next=p_list->head;
			p_list->head=new;
			if (p_list->tail==NULL)
			{
				p_list->tail=p_list->head;
			}	
		}
		else
		{
			if (insertionpoint==p_list->tail)
			{
				p_list->tail=new;
			}
			new->next=insertionpoint->next;
			insertionpoint->next=new;
		}
	}
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
	int id;
} DataInt;

void dump_int(const void *d) {
	DataInt *ptr = (DataInt*)((ListElement*)d)->data;
	printf("%d ", ptr->id);
}

void free_int(void *d) {
	free((DataInt*)((ListElement*)d)->data);
	free((ListElement*)d);
}

int cmp_int(const void *a, const void *b) {
	DataInt *p = (DataInt*)((ListElement*)a)->data;
	DataInt *q = (DataInt*)((ListElement*)b)->data;
	return p->id-q->id;
}

DataInt *create_data_int(int v) {
	DataInt *ptr = malloc(sizeof(DataInt));
	ptr->id=v;
	return ptr;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
	DataWord *ptr = (DataWord*)((ListElement*)d)->data;
	printf("%s ", ptr->word);
}

void dump_word_lowercase (const void *d) {
	DataWord *ptr = (DataWord*)((ListElement*)d)->data;
	//tolower(ptr->word);
	printf("%s ", ptr->word);
}

void free_word(void *d) {
	free((DataWord*)((ListElement*)d)->data);
	free((ListElement*)d);
}

// conpare words case insensitive
int cmp_word_alphabet(const void *a, const void *b) {
}

int cmp_word_counter(const void *a, const void *b) {
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data) {
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {

}

// test integer list
void list_test(List *p_list, int n) {
	char op[2];
	int v;
	for (int i = 0; i < n; ++i) {
		scanf("%s", op);
		switch (op[0]) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
		dump_list(p_list);
		printf("\n");
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int,
					  cmp_int, insert_elem);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word,
					  cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 0);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word,
			          cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 1);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
//			list.dump_data = dump_word_lowercase;
			dump_list_if(&list, &data);
			printf("\n");
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

