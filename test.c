#include <stdio.h>
#include <stdlib.h>

typedef struct tNode
{
    struct tNode *next;
    int value;
}Node;
void print_data(Node* lista){
    Node *ptr = (Node*)lista;
    while (ptr)
    {
        printf("%d ", ptr->value);
        ptr = ptr->next;
    }
}
void free_data(Node** t, int size){
    for (size_t i = 0; i < size; i++)
    {
        Node *tmp = (Node*)t[i];
        while (tmp)
        {
            Node *next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }
    free(t);
}

int main(){
    printf("jest to program");
    int size;
    scanf("%d", &size);
    //printf("%d ", size);
    Node **tab = malloc(size * sizeof(Node));
    int temp_size;
    for (size_t i = 0; i < size; i++)
    {
        scanf("%d ", &temp_size);
        //printf("%d ", temp_size);
        if (temp_size>0)
        {
            Node* pop = malloc(sizeof(Node));
            scanf("%d ", &pop->value);
            tab[i]=pop;
            for (size_t j = 1; j < temp_size; j++)
            {
                Node *new=malloc(sizeof(Node));
                scanf("%d ", &new->value);
                pop->next=new;
                pop=pop->next;
            }
        }
    }
    for (size_t i = 0; i < size; i++)
    {
        print_data(tab[i]->next);
    }
    
    free_data(tab, size);
    return 0;
}