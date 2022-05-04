#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A){
    return *(A + cols*row+col);
}

void set(int cols, int row, int col, double *A, double value){
    *(A + cols*row+col)=value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB){
    double suma=0;
    for (int i=0; i<rowsA; i++){
        for (int j=0; j<colsB; j++){
            suma=0;
            for (int k=0; k<colsA; k++){
                suma+=get(colsA, i, k, A)*get(colsB, k, j, B);
            }
            set(colsB, i, j, AB, suma);
        }
    }
}

void read_mat(int rows, int cols, double *t){
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            scanf("%lf", &t[i*cols+j]);
        }
    }
}

void print_mat(int rows, int cols, double *t){
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            printf("%.2lf ", get(cols, i, j, t));
        }
        printf("\n");
    }
}

int read_char_lines(char *tab[]){
    int n=-1, i=-1;
    char znak=0, line[BUF_SIZE];
    while ((znak=fgetc(stdin))!=EOF){
        if (znak==10){
            tab[n]=(char*)malloc((i+1)*sizeof(char));
            for (int x=0; x<i; x++){
                tab[n][x]=line[x];
            }
            tab[n][i]=10;
            i=-1;
            n++;
        }
        line[i]=znak;
        i++;
    }
    return n;
}

void write_char_line(char *tab[], int n){
    int x=0;
    while (tab[n-1][x]!=10){
        printf("%c", tab[n-1][x]);
        x++;
    }
}

void delete_lines(char *tab[], int line_count){
    for (int i=0; i<line_count; i++){
        free(tab[i]);
    }
}

int read_dbl_lines_v1(double *ptr_tab[]){
    char line[BUF_SIZE];
    char *ptr, *poczatek;
    int licznik=0, first=1, n=0;
    double liczba;
    while (fgets(line, BUF_SIZE, stdin)){
        poczatek=line;
        while (1){
            liczba=strtod(poczatek, &ptr);
            if (poczatek==ptr){
                break;
            }
            ptr_tab[0][licznik]=liczba;
            licznik++;
            poczatek=ptr;
        }
        n++;
        ptr_tab[n]=licznik;
    }
    return n;
}

void write_dbl_line_v1(double *ptr_tab[], int n){
    for (int i=ptr_tab[n]; i<ptr_tab[n+1]; i++){
        printf("%.2lf ", ptr_tab[0][i]);
    }
}

int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d",&rowsA,&colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d",&rowsB,&colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA,colsA,colsB,A,B,C);
			print_mat(rowsA,colsB,C);
			break;
		case 2:
			scanf("%d",&n);
			ptr_table[0] = series_table;
			lines_counter = read_dbl_lines_v1(ptr_table);
			write_dbl_line_v1(ptr_table,n);
			break;
		case 3:
			scanf("%d", &n);
			lines_counter = read_char_lines(char_lines_table);
			write_char_line(char_lines_table,n);
			delete_lines(char_lines_table,lines_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
