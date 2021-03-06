#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]);

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n){
    for (int i=0; i<m; i++){
        for (int j=0; j<n; j++){
            for (int x=0; x<p; x++){
                AB[i][j]+=A[i][x]*B[x][j];
            }
        }
    }
}

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n);

void backward_substit(double A[][SIZE], double x[], int n);

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n);

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n){
    double licznik, wyznacznik=1;
    for (int x=n-1; x>0; x--){
        for (int i=x-1; i>-1; i--){
            if (A[x][x]==0){
                return NAN;
            }
            licznik=A[x][i]/A[x][x];
            for (int j=x; j>-1; j--){
                A[j][i]-=licznik*A[j][x];
            }
        }
    }
    for(int x=0; x<n; x++){
        wyznacznik*=A[x][x];
    }
    return wyznacznik;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps){
    double wyznacznik=1, dzielnik, c[n];
    int perm[n], licznik=0;
    int maksimum, tmp;
    for(int i=0; i<n; i++){
    	perm[i]=i;
    	c[i]=b[i];
    }
    for(int i=0; i<n-1;i++){
    	maksimum=i;
    	for(int j=i+1; j<n; j++){
    		if(fabs(A[perm[maksimum]][i]) < fabs(A[perm[j]][i])){
    			maksimum=j;
    		}
    	}
	if(perm[maksimum]!=perm[i]){
		licznik+=1;
	    	tmp=perm[i];
	    	perm[i]=perm[maksimum];
	    	perm[maksimum] = tmp;
	}
    	if (fabs(A[perm[i]][i]) < eps){
    		return 0;
    	}
    	for(int j=i+1; j<n; j++){
    		dzielnik=A[perm[j]][i]/A[perm[i]][i];
    		c[perm[j]]-=dzielnik*c[perm[i]];
    		for(int k=i; k<n; k++){
    			A[perm[j]][k]-=(A[perm[i]][k]*dzielnik);
    		}
    	}
    }
    for (int i=0; i<n; i++){
    	wyznacznik*=A[perm[i]][i];
    }
    if(licznik % 2==1){
    	wyznacznik*=(-1);
    }
    for (int i=n-1; i>-1; i--){
    	for(int j=n-1; j>i; j--){
    		c[perm[i]]-=A[perm[i]][j]*c[perm[j]];
    	}
    	c[perm[i]]/=A[perm[i]][i];
    	x[i]=c[perm[i]];
    }
    return wyznacznik;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps){
    double wyznacznik=1, dzielnik, temp;
    int perm[n], licznik=0;
    int maksimum, tmp;
    for(int i=0; i<n; i++){
    	perm[i]=i;
    	B[i][i]=1;
    }
    for(int i=0; i<n-1;i++){
    	maksimum=i;
    	for(int j=i+1; j<n; j++){
    		if(fabs(A[perm[maksimum]][i]) < fabs(A[perm[j]][i])){
    			maksimum=j;
    		}
    	}
        if(perm[maksimum]!=perm[i]){
            licznik+=1;
            tmp=perm[i];
            perm[i]=perm[maksimum];
            perm[maksimum] = tmp;
        }
    	if (fabs(A[perm[i]][i]) < eps){
    		return 0;
    	}
    	for(int j=i+1; j<n; j++){
    		dzielnik=A[perm[j]][i]/A[perm[i]][i];
    		for(int k=i; k<n; k++){
    			A[perm[j]][k]-=(A[perm[i]][k]*dzielnik);
    		}
    		for(int k=0; k<n; k++){
                B[perm[j]][k]-=(B[perm[i]][k]*dzielnik);
    		}
    	}
    }
    for (int i=0; i<n; i++){
    	wyznacznik*=A[perm[i]][i];
    }
    if(licznik % 2==1){
    	wyznacznik*=(-1);
    }
    for (int i=n-1; i>-1; i--){
        for (int j=n-1; j>i; j--){
            dzielnik=A[perm[i]][j];
            A[perm[i]][j]-=dzielnik*A[perm[j]][j];
            for (int k=0; k<n; k++){
                B[perm[i]][k]-=B[perm[j]][k]*dzielnik;
            }
        }
        dzielnik=A[perm[i]][i];
        A[perm[i]][i]=1;
        for (int j=0; j<n; j++){
            B[perm[i]][j]/=dzielnik;
        }
    }
    for(int i=0; i<n; i++){
        for (int j=i+1; j<n; j++){
            if (perm[j] == i){
                for (int k=0; k<n; k++){
                    temp=B[perm[j]][k];
                    B[perm[j]][k]=B[perm[i]][k];
                    B[perm[i]][k]=temp;
                }
                tmp=perm[j];
                perm[j]=perm[i];
                perm[i]=tmp;
                break;
            }
        }
    }
    return wyznacznik;



}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if(det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A,n,n);
			printf("%.4f\n",matrix_inv(A, B, n, eps));
			print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
