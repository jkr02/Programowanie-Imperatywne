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

typedef struct _matrix{

   int rows;

   int cols;

   double**val;

   double*storage;

}Matrix;
int create_matrix(Matrix*pmatrix, int rows, int cols){
    pmatrix->rows=rows;
    pmatrix->cols=cols;
    pmatrix->storage=calloc(rows*cols, sizeof(double));
    pmatrix->val=malloc(sizeof(double)*rows);
    for (size_t i = 0; i < rows; i++)
    {
        pmatrix->val[i]=&pmatrix->storage[i*cols];
    }
    return pmatrix;
}
Matrix random_matrix(int rows,int cols){
    Matrix *macierz = malloc(sizeof(Matrix));
    macierz->rows=rows;
    macierz->cols=cols;
    macierz->storage=calloc(rows*cols, sizeof(double));
    macierz->val=malloc(sizeof(double)*rows);
    for (size_t i = 0; i < rows; i++)
    {
        macierz->val[i]=&macierz->storage[i*cols];
    }
    for (size_t i = 0; i < rows*cols; i++)
    {
        macierz->storage[i]=rand()%RAND_MAX;
    }
    
}
void copy_matrix(Matrix*ptrg,const Matrix*psrc){}
void multiply_by(Matrix*pm, double v){
    for (size_t i = 0; i < (pm->cols)*(pm->rows); i++)
    {
        pm->storage[i]*=v;
    }
}
int add(Matrix*presult,const Matrix*pm1,const Matrix*pm2){
if (pm1->cols==pm2->cols && pm1->rows==pm2->rows)
{
        presult->cols=pm1->cols;
        presult->rows=pm1->rows;
        presult->storage=calloc(presult->rows*presult->cols, sizeof(double));
        presult->val=malloc(sizeof(double)*presult->rows);
        for (size_t i = 0; i < presult->rows; i++)
        {
            presult->val[i]=&presult->storage[i*presult->cols];
        }
        for (size_t i = 0; i < presult->cols*presult->rows; i++)
        {
            presult->storage[i]=(double)pm1->storage[i]+(double)pm2->storage[i];
        }
        return 1;
    }
    return 0;
}
int main(){
    
    return 0;
}