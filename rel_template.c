#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_to_relation (pair* struktura, int size, pair para){
    for (int i=0; i<size; i++){
        if (struktura[i].first==para.first && struktura[i].second==para.second){
            return 0;
        }
    }
    return 1;
};

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* struktura, int size){
    int flag, domain[MAX_REL_SIZE];
    int dlugosc=get_domain(struktura, size, domain);
    for (int i=0; i<dlugosc; i++){
        flag=0;
        for (int j=0; j<size; j++){
            if (domain[i]==struktura[j].first && domain[i]==struktura[j].second){
                flag=1;
                break;
            }
        }
        if (flag==0){
            return 0;
        }
    }
    return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* struktura, int size){
    int flag, domain[MAX_REL_SIZE];
    int dlugosc=get_domain(struktura, size, domain);
    for (int i=0; i<dlugosc; i++){
        for (int j=0; j<size; j++){
            if (domain[i]==struktura[j].first && domain[i]==struktura[j].second){
                return 0;
            }
        }
    }
    return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* struktura, int size){
    int flag;
    for (int i=0; i<size; i++){
        flag=0;
        for (int j=0; j<size; j++){
            if (struktura[i].second==struktura[j].first && struktura[i].first==struktura[j].second){
                flag=1;
            }
        }
        if (flag==0){
            return 0;
        }
    }
    return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* struktura, int size){
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            if (struktura[i].second==struktura[j].first && struktura[i].first==struktura[j].second && struktura[i].first!=struktura[i].second){
                return 0;
            }
        }
    }
    return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* struktura, int size){
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            if (struktura[i].second==struktura[j].first && struktura[i].first==struktura[j].second){
                return 0;
            }
        }
    }
    return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* struktura, int size){
    int flag;
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            if (struktura[i].second==struktura[j].first){
                flag=0;
                for (int k=0; k<size; k++){
                    if (struktura[i].first==struktura[k].first && struktura[j].second==struktura[k].second){
                        flag=1;
                        break;
                    }
                }
                if (flag==0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* struktura, int size){
    if (is_reflexive(struktura, size)==1 && is_transitive(struktura, size)==1 && is_antisymmetric(struktura, size)==1){
        return 1;
    }
    return 0;
}
// A total order relation is a partial order relation that is connected
int is_total_order(pair* struktura, int size){
    if (is_partial_order(struktura, size)==1 && is_connected(struktura, size)==1){
        return 1;
    }
    return 0;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* struktura, int size){
    int stos=0, flag, domain[MAX_REL_SIZE];
    for (int i=0; i<size; i++){
        flag=0;
        for (int j=0; j<stos; j++){
            if (domain[j]==struktura[i].first){
                flag=1;
                break;
            }
        }
        if (flag==0){
            domain[stos]=struktura[i].first;
            stos++;
        }
        flag=0;
        for (int j=0; j<stos; j++){
            if (domain[j]==struktura[i].second){
                flag=1;
                break;
            }
        }
        if (flag==0){
            domain[stos]=struktura[i].second;
            stos++;
        }
    }
    for (int i=0; i<stos; i++){
        for (int j=1; j<stos; j++){
            if (domain[j-1]>domain[j]){
                flag=domain[j-1];
                domain[j-1]=domain[j];
                domain[j]=flag;
            }
        }
    }
    int tab[stos];
    for (int i=0; i<stos; i++){
        tab[i]=0;
    }
    for (int i=0; i<stos; i++){
        if (tab[i]==0){
            for (int j=0; j<stos; j++){
                flag=0;
                for (int k=0; k<size; k++){
                    if ((domain[i]==struktura[k].first && domain[j]==struktura[k].second)||(domain[j]==struktura[k].first && domain[i]==struktura[k].second)){
                        flag=1;
                        tab[i]=1;
                        tab[j]=1;
                        break;
                    }
                }
                if (flag==0){
                    return 0;
                }
            }
        }
    }
    return 1;
}
int find_max_elements(pair* struktura, int size, int* elementy_max){
    int stos=0, tab[size], maksimum, flag;
    for (int i=0; i<size; i++){
        tab[i]=0;
    }
    for (int i=0; i<size; i++){
        if (tab[i]==0){
            maksimum=struktura[i].second;
            for (int j=i+1; j<size; j++){
                if (struktura[j].first==struktura[i].first){
                    tab[j]=1;
                    if (struktura[j].second>maksimum){
                        maksimum=struktura[j].second;
                    }
                }
            }
            flag=0;
            for (int j=0; j<stos; j++){
                if (elementy_max[j]==maksimum){
                    flag=1;
                    break;
                }
            }
            if (flag==0){
                elementy_max[stos]=maksimum;
                stos++;
            }
        }
    }
    for (int i=0; i<stos; i++){
        for (int j=1; j<stos; j++){
            if (elementy_max[j-1]>elementy_max[j]){
                maksimum=elementy_max[j-1];
                elementy_max[j-1]=elementy_max[j];
                elementy_max[j]=maksimum;
            }
        }
    }
    return stos;
}
int find_min_elements(pair* struktura, int size, int* elementy_min){
    int stos=0, tab[size], minimum, flag;
    for (int i=0; i<size; i++){
        tab[i]=0;
    }
    for (int i=0; i<size; i++){
        if (tab[i]==0){
            minimum=struktura[i].first;
            for (int j=i+1; j<size; j++){
                if (struktura[j].second==struktura[i].second){
                    tab[j]=1;
                    if (struktura[j].first<minimum){
                        minimum=struktura[j].first;
                    }
                }
            }
            flag=0;
            for (int j=0; j<stos; j++){
                if (elementy_min[j]==minimum){
                    flag=1;
                    break;
                }
            }
            if (flag==0){
                elementy_min[stos]=minimum;
                stos++;
            }
        }
    }
    for (int i=0; i<stos; i++){
        for (int j=1; j<stos; j++){
            if (elementy_min[j-1]>elementy_min[j]){
                minimum=elementy_min[j-1];
                elementy_min[j-1]=elementy_min[j];
                elementy_min[j]=minimum;
            }
        }
    }
    return stos;
}
int get_domain(pair* struktura, int size, int* domain){
    int stos=0, flag;
    for (int i=0; i<size; i++){
        flag=0;
        for (int j=0; j<stos; j++){
            if (domain[j]==struktura[i].first){
                flag=1;
                break;
            }
        }
        if (flag==0){
            domain[stos]=struktura[i].first;
            stos++;
        }
        flag=0;
        for (int j=0; j<stos; j++){
            if (domain[j]==struktura[i].second){
                flag=1;
                break;
            }
        }
        if (flag==0){
            domain[stos]=struktura[i].second;
            stos++;
        }
    }
    for (int i=0; i<stos; i++){
        for (int j=1; j<stos; j++){
            if (domain[j-1]>domain[j]){
                flag=domain[j-1];
                domain[j-1]=domain[j];
                domain[j]=flag;
            }
        }
    }
    return stos;
}

// Case 3:
int composition (pair* struktura_a, int size_a, pair* struktura_b, int size_b, pair* relaction){
    int tab[size_b], stos=0;
    for (int i=0; i<size_b; i++){
        tab[i]=0;
    }
    for (int i=0; i<size_a; i++){
        for (int j=0; j<size_b; j++){
            if (struktura_a[i].second==struktura_b[j].first && tab[j]==0){
                tab[j]==1;
                stos++;
                break;
            }
        }
    }
    return stos;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* struktura){
    int size, indeks=0;
    pair para;
    scanf("%d", &size);
    for (int i=0; i<size; i++){
        scanf("%d", &para.first);
        scanf("%d", &para.second);
        if (add_to_relation(struktura, indeks, para)){
            struktura[indeks] = para;
            indeks++;
        }
    }
    return indeks;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

