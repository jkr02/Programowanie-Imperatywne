#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin
void wc(int *nl, int *nw, int *nc){
	int text=0, word=0;
	*nl=0;
	*nw=0;
	*nc=0;
	while((text=fgetc(stdin))!=EOF){
		*nc+=1;
		if (text==10){
			*nl+=1;
			if(word==1){
				*nw+=1;
			}
			word=0;
		}
		else if (text==32){
			if (word==1){
				*nw+=1;
			}
			word=0;
		}
		else{
			word=1;
		}
	}
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt){
	int text=0;
	int tab[LAST_CHAR-FIRST_CHAR+1];
	int indeksy[LAST_CHAR-FIRST_CHAR+1];
	for(int i=0; i<LAST_CHAR-FIRST_CHAR+1; i++){
		indeksy[i]=FIRST_CHAR+i;
		tab[i]=0;
	}
	while((text=fgetc(stdin))!=EOF){
		if(text!=10 && text!=32){
			tab[text-FIRST_CHAR]+=1;
		}
	}
	int tmp;
	int koniec=0;
	for (int i=0; i<LAST_CHAR-FIRST_CHAR; i++){
		koniec=0;
		for(int j=0; j<LAST_CHAR-FIRST_CHAR-i; j++){
			if(tab[j] < tab[j+1]){
				koniec=1;
				tmp=tab[j];
				tab[j] = tab[j+1];
				tab[j+1]=tmp;
				tmp=indeksy[j];
				indeksy[j] = indeksy[j+1];
				indeksy[j+1]=tmp;			
			}
		}
		if (koniec==0){
			break;
		}
	}
	*n_char=indeksy[char_no-1];
	*cnt=tab[char_no-1];
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[]){
	int pop=fgetc(stdin);
	int text=0;
	int indeksy[(LAST_CHAR-FIRST_CHAR+1)*(LAST_CHAR-FIRST_CHAR+1)];
	int tab[(LAST_CHAR-FIRST_CHAR+1)*(LAST_CHAR-FIRST_CHAR+1)];
	for(int i =0; i<(LAST_CHAR - FIRST_CHAR+1)*(LAST_CHAR - FIRST_CHAR+1); i++){
		indeksy[i]=i;
		tab[i]=0;
	}
	while((text=fgetc(stdin))!=EOF){
	    if(pop!=10 && text!=10 && pop!=32 && text!=32){
		    tab[(pop-FIRST_CHAR)*(LAST_CHAR - FIRST_CHAR+1)+text-FIRST_CHAR]+=1;
	    }
	    pop=text;
	}
	int tmp;
	int koniec=0;
	for (int i=0; i<(LAST_CHAR - FIRST_CHAR+1)*(LAST_CHAR - FIRST_CHAR+1); i++){
		koniec=0;
		for(int j=0; j<(LAST_CHAR - FIRST_CHAR+1)*(LAST_CHAR - FIRST_CHAR+1)-i-1; j++){
			if(tab[j] < tab[j+1]){
				koniec=1;
				tmp=tab[j];
				tab[j] = tab[j+1];
				tab[j+1]=tmp;
				tmp=indeksy[j];
				indeksy[j] = indeksy[j+1];
				indeksy[j+1]=tmp;			
			}
		}
		if (koniec==0){
			break;
		}
	}
	
	digram[0]=indeksy[digram_no-1]/(LAST_CHAR - FIRST_CHAR+1)+FIRST_CHAR;
	digram[1]=indeksy[digram_no-1]%(LAST_CHAR - FIRST_CHAR+1)+FIRST_CHAR;
	digram[2]=tab[digram_no-1];
}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter){
    int pop = fgetc(stdin);
	int text, some=0, one=0;
	*block_comment_counter=0;
	*line_comment_counter=0;
	while((text=fgetc(stdin))!=EOF){
		if( text == 10){
			one=0;
			pop=fgetc(stdin);
		}
		else{
    		if (pop==(int)'/' && text==(int)'/'&& one==0 && some==0){
    			*line_comment_counter+=1;
    			one=1;
    		}
    		else if (pop==(int)'/' && text==(int)'*' && one==0 && some==0){
    			some=1;
    		    text=fgetc(stdin);
    		}
    		else if (pop==(int)'*' && text==(int)'/' && some==1){
    			*block_comment_counter+=1;
    			some=0;
    		}
    		pop=text;
	    }
	}
}

#define MAX_LINE 128

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_line();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
			char_no = read_line();
			digram_count(char_no, digram);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
