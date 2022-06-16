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
enum pojazd{car, bus, truck};
enum napend{electric, hybrid, combustion};

struct Vehicle{
    char *name;
    int data[3];
    enum pojazd vechicle;
    enum napend engine;
    int *spec;
    switch (vechicle)
    {
    case car:
        int liczba_osob;
        float moc_silnika;
        break;
    case bus:
        int miejsca_siedzace;
        int miejsca_stojace;
    case truck:
        float pojemnosc_silnika;
        float nacisk_na_os;
    default:
        break;
    }
}
void print_pojazd(struct Vehicle *pvehicle){
    printf("%s", pvehicle->name);
    printf("%d.%d.%d ", pvehicle->data[0], pvehicle->data[1], pvehicle->data[2]);
    printf("%d", pvehicle->vechicle);
    printf("%d", pvehicle->engine);
    if (pvehicle->vechicle==0)
    {
        printf("%d %f", pvehicle->spec->liczba_osob, pvehicle->spec->moc_silnika);
    }
    else if (pvehicle->vechicle==1)
    {
        printf("%d %d", pvehicle->spec->miejsca_siedzace, pvehicle->spec->miejsca_stojace);
    }
    else if (pvehicle->vechicle==2)
    {
        printf("%f %f", pvehicle->spec->pojemnosc_silnika, pvehicle->spec->nacisk_na_os);
    }
}
    
    
    
    


}

int main(){
    
    return 0;
}