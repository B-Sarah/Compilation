#include "analyse.h"

Stack actionStack;
ScanSymbol symbol;

void ScanG0(){
    char stringRead[200];
    static int cursor = 0;
    char currentChar = '\0';
    memset(stringRead, '\0', 200 * sizeof(char));
    memset(symbol.chaine, '\0', 200 * sizeof(char));
    char cToStr[2];

    do{
        currentChar = expressionGPL[cursor];
        if(currentChar == ' ' || currentChar == '\0')
            break;
        //printf("char read : %c\n", currentChar);
        cToStr[0] = currentChar;
        cToStr[1] = '\0';
        strcat(stringRead, cToStr);
        cursor++;
    }while(currentChar != ' ' && currentChar != '\0');
    cursor = (currentChar == '\0' ? 0 : cursor+1);

    symbol.atome.aType = TER;

    symbol.atome.action = GetActionFromElement(stringRead);

    if(symbol.atome.action != 0){
        char read = '\0';
        int index = 0;

        do{
            read = stringRead[index];
            index++;
        }while(read != '#');
        strncpy(stringRead, stringRead, index - 1);
        stringRead[index - 1] = '\0';
    }

    //cas symbol
    if(isSymbol(stringRead)){
        strcpy(symbol.atome.cod.code ,stringRead);
        strcpy(symbol.chaine, stringRead);
    }
    //cas elter
    else if(stringRead[0] == '\'' && stringRead[strlen(stringRead) - 1] == '\''){
        strncpy(symbol.chaine, stringRead + 1, strlen(stringRead) - 1 - 1);
        printf("Lu un \' : %s\n", symbol.chaine);
        strcpy(symbol.atome.cod.code ,"ELETER");
    }
    //cas idnter
    else {
       symbol.atome.aType = NONTER;
       strcpy(symbol.atome.cod.code ,"IDNTER");
       strcpy(symbol.chaine, stringRead);
    }

    DisplaySymbol(symbol);
}

int AnalyseG0(PTR* ptr){
    int analyse = 0;
    switch(ptr->type){
        case CONC :
            //printf("Analyse de conc ");
            //printf("avec lu %s\n", symbol.atome.cod.code);
            if(AnalyseG0(ptr->value->conc.left)){

                analyse = AnalyseG0(ptr->value->conc.right);
                //printf("resultat conc droite %d\n", analyse);
            } else{
                analyse = 0;
                //printf("resultat conc gauche %d\n", analyse);
            }
            break;
        case UNION :
            //printf("Analyse de union partie gauche\n");
            if(AnalyseG0(ptr->value->_union.left)){
                analyse = 1;
            } else{
                //printf("Analyse de union partie droite\n");
                analyse = AnalyseG0(ptr->value->_union.right);
            }
            break;
        case STAR :
            //printf("Analyse de star\n");
            analyse = 1;
            while(AnalyseG0(ptr->value->star.stare)) ;
            //printf("Analyse star finie %d\n", analyse);
            break;
        case UN :
            analyse = 1;
            if(AnalyseG0(ptr->value->un.une)) ;
            break;
        case ATOM :{
            if(ptr->value->atom.aType == TER){
                printf("Lu un TER : %s", ptr->value->atom.cod.code);
                printf(" Scan courant : %s\n", symbol.chaine);
                //printf("Le scan respecte la grammaire ? ...\n");
                if(strcmp(ptr->value->atom.cod.code, symbol.atome.cod.code) == 0){
                    //printf("Oui\n");
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        G0Action(ptr->value->atom.action);
                    }
                    ScanG0();
                } else {
                    //printf("Non\n");
                    analyse = 0;
                }

            } else if(ptr->value->atom.aType == NONTER){
                printf("Lu un NON-TER : %s, index %d\n", ptr->value->atom.cod.code, ptr->value->atom.cod.index);

                /*if(ptr->value->atom.cod.index == -1){
                    printf("IDNTER, on passe pas\n");
                    return 0;
                }
                if(ptr->value->atom.cod.index == -2){
                    printf("IDNTER, on passe\n");
                    return 1;
                }*/
                //printf("L'analyse a marche ?...\n");
                if(AnalyseG0(treeArray[ptr->value->atom.cod.index])){
                    //printf("Oui\n");
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        G0Action(ptr->value->atom.action);
                    }

                }
                else{
                    //printf("Non\n");
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
            return i;
        }
    }
    return -1;
}

int IsTer(const char* element){
    int i;
    for(i = 0; i < 200; i++){
        if(strcmp(element, terArray[i]) == 0){
            return i;
        }
    }
    return -1;
}

void InitTerNonTerArray(){
    int i;
    for(i = 0; i < 200; i++){
        strcpy(nonTerArray[i], "");
        strcpy(terArray[i], "");
    }
}

void G0Action(int action){
    PTR *T1 = NULL, *T2 = NULL;
    int indx = 0;
    PTR* atome;
    switch(action){
        case 1:
            T1 = Pop(&actionStack);
            T2 = Pop(&actionStack);
            printf("GO index : %d\n", (T2->value->atom.cod.index));
            treeArray[(T2->value->atom.cod.index) + 5] = T1;
            break;
        case 2:
            atome = GenAtom(SearchDICONT(&indx), symbol.atome.action, NONTER);
            atome->value->atom.cod.index = indx;
            Push(&actionStack,atome);
            break;
        case 3:
            T1 = Pop(&actionStack);
            T2 = Pop(&actionStack);
            Push(&actionStack, GenUnion(T2, T1));
            break;
        case 4:
            T1 = Pop(&actionStack);
            T2 = Pop(&actionStack);
            Push(&actionStack, GenConc(T2, T1));
            break;
        case 5:
            if(symbol.atome.aType == TER){
               Push(&actionStack, GenAtom(SearchDICOT(), symbol.atome.action, TER));
            }
            else{
               atome = GenAtom(SearchDICONT(&indx), symbol.atome.action, NONTER);
               atome->value->atom.cod.index = indx;
               Push(&actionStack, atome);
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

const char* SearchDICONT(int* indx){
    int i;
    for(i = 0; i < 200; i++){

        if(strcmp(symbol.chaine, nonTerArray[i]) == 0){
            *indx = i;
            printf("WARNING !!!!!! DICO EXIST %d %s", i, nonTerArray[i]);
            return symbol.chaine;
        }
        if(strcmp(nonTerArray[i], "") == 0){
            *indx = i;
            strcpy(nonTerArray[i], symbol.chaine);
            printf("WARNING !!!!!! DICO  %d %s\n", i, nonTerArray[i]);
            return symbol.chaine;
        }

    }
    *indx = -1;
    return "";
}

const char* SearchDICOT(){
    int i;
    for(i = 0; i < 200; i++){

        if(strcmp(symbol.chaine, terArray[i]) == 0){
            return symbol.chaine;
        }
        if(strcmp(terArray[i], "") == 0){
            strcpy(terArray[i], symbol.chaine);
            return symbol.chaine;
        }

    }
    return "";
}



