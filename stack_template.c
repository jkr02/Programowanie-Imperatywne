#include <stdio.h>
#include <math.h> // for NAN

#define STACK_SIZE 10

#define OK        0 
#define EMPTY    -1
#define FULL     -2
#define OVERFLOW -3

double stack[STACK_SIZE];
int top = 0;

int stack_push(double x){
    if (top > STACK_SIZE){
        return OVERFLOW;
    }
    stack[top] = x;
    top++;
    return OK;
}

double stack_pop(void){
    if (top == 0){
        return NAN;
    }
    top--;
    return stack[top];
}

int stack_state(void){
    if (top == STACK_SIZE){
        return FULL;
    }
    return top;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

// ONE OVERFLOW for many resigning

void queue_push(int in_nr){
    for (int i=0; i<in_nr; i++){
        if (in < QUEUE_SIZE){
            curr_nr++;
            queue[in] = curr_nr;
            in++;
        }
        else{
            printf("OVERFLOW ");
            curr_nr = curr_nr + (in_nr-i);
            break;
        }
    }
}

void queue_pop(int out_nr){
    for (int i=0; i<QUEUE_SIZE; i++){
        if (i+out_nr > in){
            break;
        }
        queue[i] = queue[i+out_nr];
    }
    if (out_nr > in){
        in = 0;
        printf("UNDERFLOW ");
    }    
    in -= out_nr;
}

void queue_state(void){
    if (in == 0){
        printf("EMPTY ");
    }
    else{
        for (int i=0; i<in; i++){
            printf("%i ", queue[i]);
        }
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out=0, len=0;


void cbuff_push(int cli_nr){
    if (len == CBUFF_SIZE){
        printf("OVERFLOW ");
    }
    else{
        for (int i=0; i<CBUFF_SIZE; i++){
            if (cbuff[i] == 0){
                cbuff[i] = cli_nr;
                if (len == 0){
                    out = i;
                }
                len++;
                break;
            }
        }
    }
}

void cbuff_pop(void){
    if (len == 0){
        printf("UNDERFLOW ");
    }
    else{
        cbuff[out] = 0;
        len--;
        out = 0;
        for (int i=0; i<STACK_SIZE; i++){
            if ((cbuff[out] == 0 || cbuff[out] > cbuff[i]) && cbuff[i] != 0){
                out = i;
            }
        }
    }
}

void cbuff_state(void){
    if (len == 0){
        printf("EMPTY");
    }
    else{
        int min, p, max;
        min = cbuff[out];
        p=1;
        max = min;
        while (p == 1){
            printf("%i ", max);
            min = max;
            max = 0;
            p = 0;
            for (int i=0; i<CBUFF_SIZE; i++){
                if (cbuff[i] > min && (cbuff[i] < max || max == 0)){
                    p = 1;
                    max = cbuff[i];
                }
            }
        }
    }
}

int main(void) {
	int to_do, n, client_no, op_code;
	double x;
	scanf("%d",&to_do);
	switch(to_do) {
		case 1: // stack
			do {
				scanf("%lf",&x);
				if(x > 0) stack_push(x);
				else {
					if(x < 0) printf("%.2f ", stack_pop());
					else printf("\n%d\n", stack_state());
				} 
			} while(x != 0);
			break;
		case 2: // FIFO queue with shifts
			do {
				scanf("%d",&n);
				if(n > 0) queue_push(n);
				else {
					if(n < 0) queue_pop(-n);
					else queue_state();
				} 
			} while(n != 0);
			break;
		case 3: // queue with cyclic buffer
			client_no = 0;
			do {
				scanf("%d",&op_code);
				if(op_code > 0) cbuff_push(++client_no);
				else {
					if(op_code < 0) cbuff_pop();
					else cbuff_state();
				} 
			} while(op_code != 0);
			break;
		default: 
			printf("NOTHING TO DO!\n");
	}
	return 0;
}


