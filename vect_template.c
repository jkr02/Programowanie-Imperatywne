#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size){
    vector->data = malloc(block_size*element_size);
    vector->element_size=element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity){
    if (vector->capacity < new_capacity){
        vector->data = realloc(vector->data, new_capacity*vector->element_size);
        vector->capacity=new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void push_back(Vector *vector, void *value);
void resize(Vector *vector, size_t new_size){
    if(vector->size < new_size){
//        reserve(vector, new_size);
        int n = (int)(new_size - vector->size);
        void* v = calloc(1, vector->element_size);
//        void* v = malloc(vector->element_size);
//        memset(v, 0, vector->element_size);
        for(int i=0; i<n; i++) push_back(vector, v);
        free(v);
    }
    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value){
    if(vector->size >= vector->capacity)
        reserve(vector, 2*vector->capacity);
    char* ptr = (char*)vector->data + (vector->size * vector->element_size);
    memcpy(ptr, value, vector->element_size);
    vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector){
    free(vector->data);
    init_vector(vector, vector->capacity, vector->element_size);
}

// Remove the last element from the vector
void pop_back(Vector *vector){
    vector->size--;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value){
    push_back(vector, value);
    char* ptr = (char*)vector->data;
    ptr+=index*vector->element_size;
    memmove(ptr+vector->element_size, ptr, vector->element_size*(vector->size-index-1));
    memcpy(ptr, value, vector->element_size);
}

// Erase element at position index
void erase(Vector *vector, int index){
    if (index<vector->size){
        char* p = (char*)vector->data+vector->element_size*index;
        memmove(p, p+vector->element_size, vector->element_size*(vector->size-index));
        vector->size--;
    }
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, int(*cmp)(const void*, const void*)){
    char* ptr=(char*)vector->data;
    int a=0;
    for (int i=0; i<vector->size+a; i++){
        if (cmp(ptr, value)==0){
            erase(vector, i-a);
            a++;
        }
        else{
            ptr+=vector->element_size;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)){
    char *ptr = (char*)vector->data;
    int a=0;
    for (int i=0; i<vector->size+a; i++){
        if (predicate(ptr)==1){
            erase(vector, i-a);
            a++;
        }
        else{
            ptr+=vector->element_size;
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector){
    vector->data = realloc(vector->data, vector->element_size*vector->size);
    vector->capacity=vector->size;
}

// Print integer vector
void print_vector_int(Vector *vector){
    char *p = vector->data;
    printf("%d\n", vector->capacity);
    for (int i=0; i<vector->size; i++){
        printf("%d ", *(int*)p);
        p+=vector->element_size;
    }
}

// Print char vector
void print_vector_char(Vector *vector){
    char *ptr = vector->data;
    printf("%d\n", vector->capacity);
    for (int i=0; i<vector->size; i++){
        printf("%c ", *(char*)ptr);
        ptr+=vector->element_size;
    }
    printf("\n");
}

// Print vector of Person
void print_vector_person(Vector *vector){
    char *ptr= vector->data;
    printf("%d\n", vector->capacity);
    for (int i=0; i<vector->size; i++){
        printf("%d %s %s\n", ((Person*)ptr)->age, ((Person*)ptr)->first_name, ((Person*)ptr)->last_name);
        ptr+=vector->element_size;
    }
}

// integer comparator - increasing order
int int_cmp(const void *v1, const void *v2){
    int a=*(int*)v1;
    int b=*(int*)v2;
    if (a==b){
        return 0;
    }
    return a-b;
}

// char comparator - lexicographical order (case sensitive)
int char_cmp(const void *v1, const void *v2){
    char a = *(char*)v1;
    char b = *(char*)v2;
    return a-b;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2){
    Person* a= p1;
    Person* b= p2;
    if (a->age > b->age){
        return -1;
    }
    if (b->age > a->age){
        return 1;
    }
    if (strcmp(a->first_name, b->first_name)==0){
        return strcmp(a->last_name, b->last_name);
    }
    return strcmp(a->first_name, b->first_name);
}

// predicate: check if number is even
int is_even(void *value){
    int a = *(int*)value;
    if (a%2==0){
        return 1;
    }
    return 0;
}

// predicate: check if char is a vowel
int is_vowel(void *value){
    char a= *(char*)value;
    if (a=='a' || a=='e' || a=='i' || a=='o' || a=='u' || a=='y'){
        return 1;
    }
    if ( a=='A' || a=='E' || a=='I' || a=='O' || a=='U' || a=='Y'){
        return 1;
    }
    return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person){
    Person* p=person;
    if (p->age>25){
        return 1;
    }
    return 0;
}

// -------------------------------------------------------------

void read_int(void* value) {
	scanf("%d", (int*)value);
}

void read_char(void* value) {
	char c[2];
	scanf("%s", c);
	*(char*)value = c[0];
}

void read_person(void* value) {
	Person *person = (Person*)value;
	scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void(*read)(void*),
		 int (*cmp)(const void*, const void*), int(*predicate)(void*)) {
	char op[2];
	int index;
	size_t size;
	void *v = malloc(vector->element_size);
	for (int i = 0; i < n; ++i) {
		scanf("%s", op);
		switch (op[0]) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%d", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%d", &index);
				read(v);
				erase(vector, index);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			init_vector(&vector_int, 4, sizeof(int));
			vector_test(&vector_int, n, read_int, int_cmp, is_even);
			print_vector_int(&vector_int);
			free(vector_int.data);
			break;
		case 2:
			init_vector(&vector_char, 2, sizeof(char));
			vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
			print_vector_char(&vector_char);
			free(vector_char.data);
			break;
		case 3:
			init_vector(&vector_person, 2, sizeof(Person));
			vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
			print_vector_person(&vector_person);
			free(vector_person.data);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

