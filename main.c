#include "grammaire.h"

int main(){
    int i;
    InitArrayOfPtr();

    for(i=0; i<5; i++){
     DisplayTree(treeArray[i]);
     printf("\n");
    }
    DestroyArrayOfPtr();
	return 0;
}
