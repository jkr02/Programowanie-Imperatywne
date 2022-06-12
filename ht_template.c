#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
	int int_data;
	char char_data;
	void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef struct hash_table {
	int size;
	int no_elements;
	ht_element **ht;
	DataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->size = size;
    p_table->dump_data = dump_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->modify_data = modify_data;
    p_table->hash_function = hash_function;
    p_table->no_elements = 0;
    p_table->ht = malloc(size * sizeof(ht_element*));
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
    ht_element *ptr = p_table->ht[n];
    while(ptr){
        p_table->dump_data(ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    if(free_data && to_delete)
        free_data(to_delete->data);
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    ht_element *p, *q;
    for(int i=0; i<p_table->size; i++){
        p=p_table->ht[i];
        while(p) {
            q = p->next;
            free_element(p_table->free_data, p);
            p = q;
        }
    }
    free(p_table->ht);
}

// calculate hash function for integer k
int hash_base(int k, int size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (int)floor(size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    int old_size = p_table->size, new_hash;
    ht_element **table_copy = malloc(old_size * sizeof(ht_element*));
    memcpy(table_copy, p_table->ht, (size_t)old_size * sizeof(ht_element*));
    p_table->size *= 2;
    p_table->ht = realloc(p_table->ht, (size_t)p_table->size * sizeof(ht_element*));
    memset(p_table->ht, 0, (size_t)p_table->size * sizeof(ht_element*));
    ht_element *p, *q;
    for(int i=0; i < old_size; i++){
        p = table_copy[i];
        while(p){
            q = p->next;
            new_hash = p_table->hash_function(p->data, p_table->size);
            p->next = p_table->ht[new_hash];
            p_table->ht[new_hash] = p;
            p = q;
        }
    }
    free(table_copy);
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {
    ht_element *ptr = p_table->ht[*first];
    if(!ptr) return NULL;
    while(ptr->next && p_table->compare_data(ptr->next->data, data) != 0)
        ptr = ptr->next;
    if (ptr->next) return ptr;
    return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    int hash_val = p_table->hash_function(*data, p_table->size);
    ht_element *ptr = p_table->ht[hash_val];
    while(ptr && p_table->compare_data(ptr->data, *data) != 0)
        ptr = ptr->next;
    if(ptr) return ptr;
    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    int hash_val = p_table->hash_function(*data, p_table->size);
    ht_element *ptr = get_element(p_table, data);
    if (!ptr) {
        ht_element *new = malloc(sizeof(ht_element));
        ptr = p_table->ht[hash_val];
        new->data = *data;
        new->next = ptr;
        p_table->ht[hash_val] = new;
        p_table->no_elements++;
        if (((float) p_table->no_elements / (float) p_table->size) > MAX_RATE)
            rehash(p_table);
    }
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    int hash_val = p_table->hash_function(data, p_table->size);
    ht_element *ptr = find_previous(p_table, data, &hash_val);
    if(ptr){
        ht_element *ptr_next = ptr->next;
        ptr->next = ptr->next->next;
        free_element(p_table->free_data, ptr_next);
        p_table->no_elements--;
    }
    else{
        ptr = p_table->ht[hash_val];
        if(p_table->compare_data(ptr->data, data) == 0){
            p_table->ht[hash_val] = ptr->next;
            free_element(p_table->free_data, ptr);
            p_table->no_elements--;
        }
    }
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
    int val;
    data_union new_union;
    scanf("%d", &val);
    new_union.int_data = val;
    return new_union;
}

// char element

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return a.char_data - b.char_data;
}

// read char value and insert to the union
data_union create_data_char() {
    char c;
    data_union new_data;
    scanf(" %c", &c);
    new_data.char_data = c;
    return new_data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord* dw = (DataWord*)(data.ptr_data);
    printf("%s %d\n", dw->word, dw->counter);
}

void free_word(data_union data) {
    DataWord* dw = (DataWord*)(data.ptr_data);
    if(dw && dw->word) free(dw->word);
    free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
    DataWord* a_dw = (DataWord*)(a.ptr_data);
    DataWord* b_dw = (DataWord*)(b.ptr_data);
    return strcmp(a_dw->word, b_dw->word);
}

int hash_word(data_union data, int size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
    DataWord* dw = (DataWord*)((*data).ptr_data);
    dw->counter++;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
    DataWord *dw = malloc(sizeof(DataWord));
    dw->word = strdup(value);
    dw->counter = 1;
    data_union new_union;
    new_union.ptr_data = dw;
    return new_union;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char BUFF[BUFFER_SIZE];
    char corrector[] = ".,?!:;-\n\t\r ";
    char *stash;
    int n;
    data_union new_data;
    ht_element *ptr;
    while(fgets(BUFF, BUFFER_SIZE, stream)){
        stash = strtok(BUFF, corrector);
        while(stash != NULL )
        {
            n = (int)strlen(stash);
            for(int i=0; i<n; i++)
                stash[i] = (char)tolower(stash[i]);
            new_data = create_data_word(stash);
            ptr = get_element(p_table, &new_data);
            if(ptr) modify_word(&ptr->data);
            else insert_element(p_table, &new_data);
            stash = strtok(NULL, corrector);
        }
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = create_data();
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n, index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_int,NULL,
					cmp_int, hash_int, NULL);
			table_test(&table, n, create_data_int);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 2: // test char table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_char,NULL,
			        cmp_char, hash_char, NULL);
			table_test(&table, n, create_data_char);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 3: // read words from text, insert into table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, free_word,
			        cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%d\n", table.size);
			data = create_data_word(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
