#include "grammaire.h"

char expression[200]= "S -> [ a ] . b , ;";
const char* nonTerArray[200];
const char* terArray[200];

int main(){
    int i;

    InitArrayOfPtr();
    InitTerNonTerArray();

    nonTerArray[0] = "S";
    nonTerArray[1] = "T";

    terArray[0] = "a";
    terArray[1] = "b";

    for(i=0; i<5; i++){
     DisplayTree(treeArray[i]);
     printf("\n");
    }

    Scan();
    Scan();

    if(Analyse(treeArray[S]))
        printf("OK\n");
    else
        printf("Pas OK\n");

    printf("\n");

    for(i=5; i<50; i++){
        if(treeArray[i] != NULL){
            DisplayTree(treeArray[i]);
            printf("\n");
        }
    }

    DestroyArrayOfPtr();

	return 0;
}
