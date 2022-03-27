#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};
int jest_poczatek(char x){
    if((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || x == '_'){
        return 1;
    }
    return 0;
}
int jest_srodek(char x){
    if((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || x == '_' || (x >= '0' && x <= '9')){
        return 1;
    }
    return 0;
}

int jest_keyword(char* x, int c){
    int znajduje=0;
    for(int i=0; i<32; i++){
        if (strlen(keywords[i]) == c){
            for(int a=0; a<strlen(keywords[i]); a++){
                if ((int)x[a]==(int)keywords[i][a]){
                    znajduje=1;
                }
                else{
                    znajduje=0;
                    break;
                }
            }
            if(znajduje==1){
                return 1;
            }
        }
        return 0;
    }
}


int find_idents(){
    int kom_bl=0, kom_lin=0, p_ap=0, m_ap=0, indeks=0, n, start=0, j, wysokosc=0, jest=0, znajduje=0;
    char linia[128], word[128], tablica[MAX_IDS][128];
    while(fgets(linia, 128, stdin)){
        n=strlen(linia);
        for(int i=0; i<n; i++){
            if (kom_lin==0 && kom_bl==0 && p_ap==0 && m_ap==0 && linia[i]!=32 &&linia[i]!=10 &&jest_srodek(linia[i])){
                if(start==0 && jest_poczatek(linia[i])){
                    word[0]=linia[i];
                    j=1;
                    start=1;
                }
                else if(start==1){
                    word[j]=linia[i];
                    j++;
                }
            }
            else if (start==1){
                for(int x=0; x<32; x++){
    		            if (strlen(keywords[x]) == j){
        		            for(int y=0; y<strlen(keywords[x]); y++){
        		                if ((int)word[y]==(int)keywords[x][y]){
        		                    znajduje=1;
        		                }
        		                else{
        		                    znajduje=0;
        		                    break;
        		                }
        		            }
        		            if(znajduje==1){
        		                break;
        		            }
    		            }
    		            }
    		            if (znajduje==0){
    		                for (int x=0;x<wysokosc;x++){
    		                    if (strcmp(word, tab[x])==0){
    		                        znajduje=1;
    		                        break;
    		                    }
    		                }
    		                if(znajduje==0){
    		                    for (int x=0; x<128; x++){
    		                        tab[wysokosc][x]=word[x];
    		                    }
    		                    wysokosc+=1;
    		                }
    		            }
                znajduje=0;
                memset(&word[0], 0, sizeof(word));
                start=0;
            }
            if(p_ap + m_ap > 0 && linia[i] == '\\'){
                i++;
            }
            else if(p_ap + m_ap + kom_bl == 0 && linia[i] == '/'){
                if(linia[i+1] == '/')
                    break;
                if(linia[i+1] == '*') {
                    kom_bl++;
                    i++;
                }
            }
            else if(p_ap + m_ap == 0 && linia[i] == '*' && linia[i+1] == '/') {
                kom_bl--;
                i++;
            }
            else if(kom_bl + p_ap == 0 && linia[i] == '\"'){
                if(m_ap == 0) m_ap++;
                else m_ap--;
            }
            else if(kom_bl + m_ap == 0 && linia[i] == '\''){
                if(p_ap == 0) p_ap++;
                else p_ap--;
            }
        }
    }
    for (int x=0; x<wysokosc; x++){
	    for(int y=0; y<MAX_ID_LEN; y++){
	        printf("%c", tab[x][y]);
	    }
	    printf("\n");
	}
    return wysokosc;
}


int cmp(const void* first_arg, const void* second_arg) {
    char *a = *(char**)first_arg;
    char *b = *(char**)second_arg;
    return strcmp(a, b);
}


int index_cmp(const void* first_arg, const void* second_arg) {
    int a = *(int*)first_arg;
    int b = *(int*)second_arg;
    return strcmp(tab[a], tab[b]);
}


int main(void) {
    printf("%d\n", find_idents());
    return 0;
}
