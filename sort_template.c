#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2){
    Person *c1 = (Person *)p1;
    Person *c2 = (Person *)p2;
    if (c1->age < c2->age){return 1;}
    else if (c1->age > c2->age){return -1;}
    else{
        if (strcmp(c1->first_name, c2->first_name)==0){
            return strcmp(c1->last_name, c2->last_name);
        }
        return strcmp(c1->first_name, c2->first_name);
    }

}

// Read data to Person array (till EOF)
int read_person_array(Person *persons){
    int n=0;
    char line[134];
    while (fgets(line, 134, stdin)){
        if (line[0]!=10){
        sscanf(line, "%d %s %s ", &persons[n].age, persons[n].first_name, persons[n].last_name);
        //printf("%d %s %s ", wiek, first, last);
        n++;
        }
    }
    return n;
    //printf("\n\n%d %s %s", persons[2].age, persons[2].first_name, persons[2].last_name);
}

// Print Person array
void print_person_array(Person *persons, int n){
    for (int i=0; i<n; i++){
        printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);
    }
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2){
    int age_a, age_b, i=1;
    char plec_a, plec_b, first_a[MAX_STR_LEN], first_b[MAX_STR_LEN], last_a[MAX_STR_LEN], last_b[MAX_STR_LEN];
    sscanf(l1, "%d %s %s ", &age_a, first_a, last_a);
    sscanf(l2, "%d %s %s ", &age_b, first_b, last_b);
    while (first_a[i]!=0){
        plec_a=first_a[i];
        i++;
    }
    i=1;
    while (first_b[i]!=0){
        plec_b=first_b[i];
        i++;
    }
    if (plec_a=='a'){
        if (plec_b=='a'){
            if (age_a < age_b){return -1;}
            if (age_a > age_b){return 1;}
            return 0;
        }
        return -1;
    }
    if (plec_b=='a'){return 1;}
    return strcmp(last_a, last_b);


}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]){
    char line[MAX_STR_LEN];
    int n=0;
    while(fgets(line, MAX_STR_LEN, stdin)){
        if (line[0]!=10){
            strcpy(lines[n], line);
            n++;
        }
    }
    return n;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n){
    for (int i=0; i<n; i++){
        printf("%s", lines[i]);
    }
}

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}

