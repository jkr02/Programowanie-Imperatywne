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

typedef struct  {

    double *results;

    int len;

} Data;
typedef struct tagQueueElement {
    Data data;
    QueueElement *next;
} QueueElement;
typedef struct tagQueue {
    QueueElement *first;
    QueueElement *last;
} Queue;
int free_queue(Queue *pqueue, int **table){

}
int pop(Queue *pqueue,  Data*pdata){
    if (pqueue->first==NULL)
    {
        return -1;
    }
    QueueElement *ptr=(QueueElement*)pqueue->first;
    if (pqueue->first==pqueue->last)
    {
        pqueue->first=NULL;
        pqueue->last=NULL;
    }
    else
    {
        pqueue->first=pqueue->first->next;
    }
    pdata=ptr->data;
    free(ptr->data);
    free(ptr);
    if (pqueue->first==NULL)
    {
        return 0;
    }
    return 1;
}
void push_copy(Queue *pqueue, const Data*pdata){
    QueueElement *new = malloc(sizeof(QueueElement));
    Data *new_data = malloc(sizeof(Data));
    new_data->len=pdata->len;
    new_data->results=pdata->results;
    new->data=new_data;
    if (pqueue->first==NULL)
    {
        pqueue->first=new;
        pqueue->last=new;
    }
    else{
        QueueElement *ptr = (QueueElement*)pqueue->last;
        pqueue->last=new;
        ptr->next=new;
    }
}

int get_total_count(const Queue *pqueue){
    int dlugosc=0;
    QueueElement *ptr=(QueueElement*)pqueue->first;
    while (ptr)
    {
        dlugosc+=((Data*)ptr->data)->len;
        ptr=ptr->next;
    }
    return dlugosc;
}

int main(){
    
    return 0;
}