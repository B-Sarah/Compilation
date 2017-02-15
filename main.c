#include "grammaire.h"

char expression[200]= "S -> [ a ] . b";
const char* nonTerArray[200];

int main(){
    int i;
    InitArrayOfPtr();
    nonTerArray[0] = "S";

    for(i=0; i<5; i++){
     DisplayTree(treeArray[i]);
     printf("\n");
    }
    Analyse(treeArray[S]);

    DestroyArrayOfPtr();

	return 0;
}
