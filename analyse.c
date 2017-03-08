#include "analyse.h"

Stack actionStack;
ScanSymbol symbol;

void Scan(){
    char stringRead[10];
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

    printf("Scan a lu : %s\n", stringRead);
    if(IsNonTer(stringRead)){
       symbol.atome.aType = NONTER;
       strcpy(symbol.atome.cod.code ,"IDNTER");
    }else if(IsTer(stringRead)){
        symbol.atome.aType = TER;
        strcpy(symbol.atome.cod.code ,"ELETER");
    }
    else{
        symbol.atome.aType = TER;
        strcpy(symbol.atome.cod.code, stringRead);
    }
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
            if(ptr->value->atom.aType == TER){
                printf("Lu un TER : %s\n", ptr->value->atom.cod.code);
                printf("Scan courant : %s\n", symbol.chaine);
                printf("Le scan respecte la grammaire ? ...\n");
                if(strcmp(ptr->value->atom.cod.code, symbol.atome.cod.code) == 0){
                    printf("Oui\n");
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        G0Action(ptr->value->atom.action);
                    }
                    Scan();
                } else {
                    printf("Non\n");
                    analyse = 0;
                }

            } else if(ptr->value->atom.aType == NONTER){
                printf("Lu un NON-TER : %s, index %d\n", ptr->value->atom.cod.code, ptr->value->atom.cod.index);

                if(ptr->value->atom.cod.index == -1){
                    printf("IDNTER, on passe pas\n");
                    return 0;
                }
                if(ptr->value->atom.cod.index == -2){
                    printf("IDNTER, on passe\n");
                    return 1;
                }
                printf("L'analyse a marche ?...\n");
                if(Analyse(treeArray[ptr->value->atom.cod.index])){
                    printf("Oui\n");
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        G0Action(ptr->value->atom.action);
                    }

                }
                else{
                    printf("Non\n");
                    analyse = 0;
                }

            } else{
                printf("Erreur dans l'analyse\n");
            }
            break;
        }
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
    printf("- cod : %s\n- action : %d\n- type : %s\n- chaine : %s\n", symbol.atome.cod.code, symbol.atome.action, DisplayAtom(symbol.atome.aType), symbol.chaine);
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

int IsTer(const char* element){
    int i;
    for(i = 0; i < 200; i++){
        if(strcmp(element, terArray[i]) == 0){
            return 1;
        }
    }
    return 0;
}

void InitTerNonTerArray(){
    int i;
    for(i = 0; i < 200; i++){
        nonTerArray[i] = "";
        terArray[i] = "";
    }
}

void G0Action(int action){
    PTR *T1, *T2;
    switch(action){
        case 1:
            T1 = Pop(&actionStack);
            T2 = Pop(&actionStack);
            printf("index : %d", (T2->value->atom.cod.index));
            treeArray[(T2->value->atom.cod.index) + 5] = T1;
            break;
        case 2:
            Push(&actionStack, GenAtom(Search(nonTerArray), action, NONTER));
            break;
        case 3:
            T1 = Pop(&actionStack);
            T2 = Pop(&actionStack);
            Push(&actionStack, GenUnion(T1, T2));
            break;
        case 4:
            T1 = Pop(&actionStack);
            T2 = Pop(&actionStack);
            Push(&actionStack, GenConc(T1, T2));
            break;
        case 5:
            if(symbol.atome.aType == TER){
               Push(&actionStack, GenAtom(Search(terArray), action, TER));
            }
            else{
               Push(&actionStack, GenAtom(Search(nonTerArray), action, NONTER));
            }
            break;
        case 6:
            T1 = Pop(&actionStack);
            Push(&actionStack, GenStar(T1));
            break;
        case 7:
            T1 = Pop(&actionStack);
            Push(&actionStack, GenUn(T1));
            break;


    }

}

const char* Search(const char* DICO[]){
    int i;
    for(i = 0; i < 200; i++){
        if(strcmp(symbol.chaine, DICO[i]) == 0){
            return symbol.chaine;
        }
        if(strcmp(DICO[i], "")){
            DICO[i] = symbol.chaine;
            return symbol.chaine;
        }

    }
    return "";

}



