#include "grammaire.h"
PTR* treeArray[5];
const char* g0NonTerArray[5];


PTR* GenConc(PTR* p1, PTR* p2){
	PTR* p = NULL;

	if(Allocate(CONC,&p) == 0){
        p->type = CONC;
        p->value->conc.left = p1;
        p->value->conc.right = p2;
	}
	return p;
}
PTR* GenUnion(PTR* p1, PTR* p2){
	PTR* p = NULL;

	if(Allocate(UNION,&p) == 0){
	    p->type = UNION;
        p->value->_union.left = p1;
        p->value->_union.right = p2;
	}
	return p;
}
PTR* GenStar(PTR* p1){
	PTR* p = NULL;

	if(Allocate(STAR,&p)== 0){
        p->type = STAR;
        p->value->star.stare = p1;
    }
	return p;
}
PTR* GenUn(PTR* p1){
	PTR* p = NULL;

	if(Allocate(UN,&p) == 0){
        p->type = UN;
        p->value->un.une = p1;
    }
	return p;
}



PTR* GenAtom(const char* cod, int action, AtomType aType){
	static int index = 1;
	PTR* p = NULL;
	printf("gen atom : cod = %s\n", cod);
	if(Allocate(ATOM,&p) == 0){
        	p->type = ATOM;
	        strncpy(p->value->atom.cod.code, cod, 9);
	        p->value->atom.action = action;
        	p->value->atom.aType = aType;

        	if(aType == NONTER && elementMustBeAdded(cod)){
	            if(!elementAlreadyRead(cod)){
        	        g0NonTerArray[index] = cod;
                	p->value->atom.cod.index = index;
	                printf("non ter array : cod = %s __ index = %d\n", cod, index);
        	        index++;
            	    }
        	}
    	}
	return p;
}

int elementAlreadyRead(const char* element){
    int i;
    for(i = 0; i < 5; i++){
        if(strcmp(g0NonTerArray[i],element)==0){
            return 1;
        }
    }
    return 0;
}

int elementMustBeAdded(const char* element){
    return strcmp(element, "IDNTER") != 0 && strcmp(element, "ELTER") != 0;
}


int Allocate(Operation operation, PTR **p){
    size_t p_Size;
    __PTR* ptr = NULL;
    PTR* tmp = NULL;
    switch(operation){
    case CONC :
        p_Size = sizeof(Conc);
        break;
    case UNION :
        p_Size = sizeof(Union);
        break;
    case STAR :
        p_Size = sizeof(Star);
        break;
    case UN :
        p_Size = sizeof(Un);
        break;
    case ATOM :
        p_Size = sizeof(Atom);
        break;
    }
    ptr = (__PTR*) malloc(p_Size);
    if(ptr == NULL) return -1;

    if(operation == ATOM)
        memset(ptr->atom.cod.code, '\0', 10 * sizeof(char));

    tmp = (PTR*) malloc(p_Size+sizeof(Operation));
    //printf("P : %p", p);
    if(tmp == NULL){
        free(ptr);
        return -2;
    }
    tmp->value = ptr;
    *p = tmp;

    return 0;
}



int DestroyPtr(PTR** ptr){
    if((*ptr)->type == CONC){
        DestroyPtr(&(*ptr)->value->conc.left);
        DestroyPtr(&(*ptr)->value->conc.right);
    }
    else if((*ptr)->type == UNION) {
        DestroyPtr(&(*ptr)->value->_union.left);
        DestroyPtr(&(*ptr)->value->_union.right);
    }
    else if((*ptr)->type == STAR) {
        DestroyPtr(&(*ptr)->value->star.stare);
    }
    else if((*ptr)->type == UN) {
        DestroyPtr(&(*ptr)->value->un.une);
    }
    free((*ptr)->value);
    (*ptr)->value = NULL;
    free(*ptr);
    *ptr = NULL;
    return 0;
}

const char* DisplayAtom(AtomType atom){
    switch(atom){
    case TER :
        return "TER";
    case NONTER :
        return "NONTER";
    }
    return "";
}


void DisplayTree(PTR* ptr){
__DisplayTree(ptr,1);
}

void __DisplayTree(PTR* ptr, int _count){
      if(ptr->type == CONC){
        printRepeatedChar('-', _count*2);
        printf("Conc \n");
        __DisplayTree(ptr->value->conc.left,_count+1);
        __DisplayTree(ptr->value->conc.right,_count+1);
    }
    else if(ptr->type == UNION) {
        printRepeatedChar('-', _count*2);
        printf("Union \n");
        __DisplayTree(ptr->value->_union.left,_count+1);
        __DisplayTree(ptr->value->_union.right,_count+1);
    }
    else if(ptr->type == STAR) {
        printRepeatedChar('-', _count*2);
        printf("Star \n");
        __DisplayTree(ptr->value->star.stare,_count+1);
    }
    else if(ptr->type == UN) {
        printRepeatedChar('-', _count*2);
        printf("Un \n");
        __DisplayTree(ptr->value->un.une,_count+1);
    }
    else if(ptr->type == ATOM) {
        printRepeatedChar('-', _count*2);
        printf("Atom %s %d %s \n",ptr->value->atom.cod.code, ptr->value->atom.action, DisplayAtom(ptr->value->atom.aType));

    }
}

void printRepeatedChar(char c, int cnt){
    int i;
    for(i = 0; i < cnt; i++){
        printf("%c", c);
    }
}

void DestroyArrayOfPtr(){
    int i;
    for(i=0; i<5; i++){
        DestroyPtr(&treeArray[i]);
    }
}

void InitArrayOfPtr(){
    initNonTerArray();

    treeArray[0] = GenConc(GenStar(GenConc(GenConc(GenConc(GenAtom("N",0,NONTER),GenAtom("->",0,TER)),GenAtom("E",0,NONTER)),GenAtom(",",1,TER))),GenAtom(";",0,TER));
    treeArray[1] = GenAtom("IDNTER",0,NONTER);
    treeArray[2] = GenConc(GenAtom("T",0,NONTER),GenStar(GenConc(GenAtom("+",0,TER),GenAtom("T",0,NONTER))));
    treeArray[3] = GenConc(GenAtom("F",0,NONTER),GenStar(GenConc(GenAtom(".",0,TER),GenAtom("F",0,NONTER))));
    treeArray[4] = GenUnion(GenUnion(GenUnion(GenUnion(GenAtom("IDNTER",0,NONTER),GenAtom("ELTER",0,NONTER)),
                                             GenConc(GenConc(GenAtom("(",0,TER),GenAtom("E",0,NONTER)),
                                                     GenAtom(")",0,TER))),GenConc(GenConc(GenAtom("[",0,TER),GenAtom("E",0,NONTER)),
                                                                                 GenAtom("]",0,TER))),GenConc(GenConc(GenAtom("[|",0,TER),GenAtom("E",0,NONTER)),GenAtom("|]",0,TER)));
}

void initNonTerArray(){
    int i;
    for(i = 0; i < 5; i++){
        g0NonTerArray[i] = "";
    }
    g0NonTerArray[0] = "S";

    for(i = 0; i < 200; i++){
        nonTerArray[i] = "";
    }
}

int IsNonTer(const char* element){
    int i;
    for(i = 0; i < 200; i++){
        if(strcmp(element, nonTerArray[i]) == 0){
            return 1;
        }
    }
    return 0;
}

