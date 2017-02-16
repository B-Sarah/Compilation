#include "analyse.h"

char stringRead[10];

void Scan(){
    static int cursor = 0;
    char currentChar = '\0';
    memset(stringRead, '\0', 10 * sizeof(char));
    char cToStr[2];

    do{
        currentChar = expression[cursor];
        if(currentChar == ' ' || currentChar == '\0')
            break;
        //printf("char read : %c\n", currentChar);
        cToStr[0] = currentChar;
        cToStr[1] = '\0';
        strcat(stringRead, cToStr);
        cursor++;
    }while(currentChar != ' ' && currentChar != '\0');
    //printf("read : %s\n", stringRead);

    cursor = (currentChar == '\0' ? 0 : cursor+1);

    ScanSymbol symbol;
    if(IsNonTer(stringRead)){
       symbol.atome.aType = NONTER;
    }else{
        symbol.atome.aType = TER;
    }
    strcpy(symbol.atome.cod.code, stringRead);
    symbol.atome.action = GetActionFromElement(stringRead);
    strcpy(symbol.chaine, stringRead);

    DisplaySymbol(symbol);
}

int Analyse(PTR* ptr){
    int analyse = 0;
    switch(ptr->type){
        case CONC :
            if(Analyse(ptr->value->conc.left)){
                analyse = Analyse(ptr->value->conc.right);
            } else{
                analyse = 0;
            }
            break;
        case UNION :
            if(Analyse(ptr->value->_union.left)){
                analyse = 1;
            } else{
                analyse = Analyse(ptr->value->_union.right);
            }
            break;
        case STAR :
            analyse = 1;
            while(Analyse(ptr->value->star.stare)) ;
            break;
        case UN :
            analyse = 1;
            if(Analyse(ptr->value->un.une)) ;
            break;
        case ATOM :{
		const char* cStr = ptr->value->atom.cod.code;
                int index = ptr->value->atom.cod.index;

            if(ptr->value->atom.aType == TER){
		 printf("TER : code cod %s __ code index %d\n", cStr, index);
		printf("StringRead : %s\n", stringRead);
                if(strcmp(ptr->value->atom.cod.code, stringRead) == 0){
		    printf("strcmp success\n");
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        //G0Action(ptr->value->atom.action);
                    }
                    Scan();
                } else {
                    analyse = 0;
                }

            } else if(ptr->value->atom.aType == NONTER){
                printf("AVANT : code cod %s __ code index %d\n", cStr, index);
                if(1/*Analyse(treeArray[ptr->value->atom.cod.index])*/){
		printf("APRES\n");
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        //G0Action(ptr->value->atom.action);
                    }
			Scan();
                }
                else{
                    analyse = 0;
                }

            }else{
		printf("Erreur dans l'analyse\n");
		}
            break;}
    }
    return analyse;
}

int GetActionFromElement(const char* element){
    char read = '\0';
    int index = 0;
    char code[5] = {'\0'};
    int action = 0;

    do{
        read = element[index];
        index++;
    }while(read != '\0' && read != '#');

    if(read == '#'){
        strncpy(code, element + index, strlen(element) - index);
        action = atoi(code);
    }

    return action;
}

void DisplaySymbol(ScanSymbol symbol){
    printf("cod : %s\naction : %d\ntype : %s\nchaine : %s\n", symbol.atome.cod.code, symbol.atome.action, DisplayAtom(symbol.atome.aType), symbol.chaine);
}
