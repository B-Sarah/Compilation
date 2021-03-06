#include "gpl.h"

char languageSymbols[100][10];
char keywords[100][200];
TableVar tableVar;
GPLSymbol gplSymbol;
GPLSymbol prevGplSymbol;
PCode pcode;
PileT pTravail;

int currentNumberIndex = 0;
int numberIsRead = 0;

int currentStringIndex = 0;
int stringReadLength = 0;
int stringIsRead = 0;

void ScanGPL(){
    storeToLastSymbol();

    char stringRead[200];
    static int cursor = 0;
    char currentChar = '\0';
    memset(stringRead, '\0', 200 * sizeof(char));
    char cToStr[2];

    static char numberString[50];
    static char stringString[200];

    do{
        currentChar = program[cursor];
        if(currentChar == ' ' || currentChar == '\0')
            break;
        cToStr[0] = currentChar;
        cToStr[1] = '\0';
        strcat(stringRead, cToStr);
        cursor++;
    }while(currentChar != ' ' && currentChar != '\0');

    cursor = (currentChar == '\0' ? 0 : cursor+1);
    memset(gplSymbol.strRead, '\0', 200 * sizeof(char));

    int nb;
    if((nb = isNumber(stringRead)) != 0) {
        //1ere fois qu'on lis le nombre
        if(numberIsRead == 0){
            memset(numberString, 0, 50 * sizeof(char));
            strcpy(numberString, stringRead);
            currentNumberIndex = 0;
            numberIsRead = 1;

            if(nb == 1){
                gplSymbol.type = ENT;
                gplSymbol.value.integer = atoi(numberString);
            }
            else{
                gplSymbol.type = DBL;
                gplSymbol.value.dbl = atof(numberString);
            }
        }

        gplSymbol.strRead[0] = numberString[currentNumberIndex];
        gplSymbol.strRead[1] = '\0';
        currentNumberIndex++;

        if(currentNumberIndex == strlen(numberString)){
            numberIsRead = 0;
        } else {
            cursor -= (strlen(stringRead) + 1);
        }

    }
    else if(isGplSymbol(stringRead)){
        memset(gplSymbol.value.symbol, 0, 5 * sizeof(char));
        strcpy(gplSymbol.value.symbol, stringRead);
        strcpy(gplSymbol.strRead, stringRead);
        gplSymbol.type = SYMBOL;
    } else if(isKeyword(stringRead)){
        memset(gplSymbol.value.ident, 0, 200 * sizeof(char));
        strcpy(gplSymbol.value.ident, stringRead);
        strcpy(gplSymbol.strRead, stringRead);
        gplSymbol.type = KEYWORD;
    } else {
        if(stringIsRead == 0){
            memset(stringString, 0, 200 * sizeof(char));
            strcpy(stringString, stringRead);
            currentStringIndex = 0;
            stringIsRead = 1;
            stringReadLength = strlen(stringRead);

            gplSymbol.type = IDENT;
            memset(gplSymbol.value.ident, 0, 200 * sizeof(char));
            strcpy(gplSymbol.value.ident, stringRead);
        }

        gplSymbol.strRead[0] = stringString[currentStringIndex];
        gplSymbol.strRead[1] = '\0';
        currentStringIndex++;

        if(currentStringIndex == strlen(stringString)){
            stringIsRead = 0;
        } else {
                cursor -= (strlen(stringRead) + 1);
        }
    }

    //DisplayGPLSymbol(gplSymbol);
}

void DisplayGPLSymbol(GPLSymbol s){
    printf("Scan GPL :\n");
    printf("- chaine lue : %s\n", gplSymbol.strRead);
    printf("- type : %d\n", gplSymbol.type);
    switch(gplSymbol.type){
    case IDENT:
        printf("- identificateur : %s\n", gplSymbol.value.ident);
        break;
    case KEYWORD:
        printf("- mot cle : %s\n", gplSymbol.value.ident);
        break;
    case ENT:
        printf("- entier : %d\n", gplSymbol.value.integer);
        break;
    case DBL:
        printf("- double : %lf\n", gplSymbol.value.dbl);
        break;
    case SYMBOL:
        printf("- symbole : %s\n", gplSymbol.value.symbol);
        break;
    }
    //printf("- cod : %s\n- action : %d\n- type : %s\n- chaine : %s\n", symbol.atome.cod.code, symbol.atome.action, DisplayAtom(symbol.atome.aType), symbol.chaine);
}

int AnalyseGPL(PTR* ptr){
    int analyse = 0;
    if(ptr == NULL){ printf("ERREUR - GPL incorrecte !\n"); return 0;}
    switch(ptr->type){
        case CONC :
            //printf("analyse conc gauche\n");
            if(AnalyseGPL(ptr->value->conc.left)){
                //printf("analyse conc droite\n");
                analyse = AnalyseGPL(ptr->value->conc.right);
                //printf("resultat conc droite : %d\n", analyse);
            } else{
                analyse = 0;
                //printf("resultat conc gauche : %d\n", analyse);
            }
            break;
        case UNION :
            //printf("analyse union gauche\n");
            if(AnalyseGPL(ptr->value->_union.left)){
                analyse = 1;
                //printf("resultat analyse gauche : %d\n", analyse);
            } else{
                //printf("analyse union droite\n");
                analyse = AnalyseGPL(ptr->value->_union.right);
                //printf("resultat analyse droite : %d\n", analyse);
            }
            break;
        case STAR :
            analyse = 1;
            while(AnalyseGPL(ptr->value->star.stare)){
                //printf("analyse star");
            }
            break;
        case UN :
            analyse = 1;
            if(AnalyseGPL(ptr->value->un.une)) ;
            break;
        case ATOM :{
            if(ptr->value->atom.aType == TER){
                //printf("Attendu : %s Lu : %s\n", ptr->value->atom.cod.code, gplSymbol.strRead);
                if(strcmp(ptr->value->atom.cod.code, gplSymbol.strRead) == 0){
                    //printf("le ter %s a ete analyse avec succes\n", ptr->value->atom.cod.code);
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        GPLAction(ptr->value->atom.action);
                    }
                    ScanGPL();
                } else {
                    //printf("echec d'analyse du ter %s\n", ptr->value->atom.cod.code);
                    analyse = 0;
                }

            } else if(ptr->value->atom.aType == NONTER){
                //printf("Lecture non ter : %s on rendre dedans\n", ptr->value->atom.cod.code);
                if(strcmp(ptr->value->atom.cod.code, "int") == 0 && gplSymbol.type == DBL){
                    analyse = 0;
                    break;
                }
                if(AnalyseGPL(treeArray[ptr->value->atom.cod.index + 5])){
                    //printf("le non ter %s a ete analyse avec succes\n", ptr->value->atom.cod.code);
                    analyse = 1;
                    if(ptr->value->atom.action != 0){
                        GPLAction(ptr->value->atom.action);
                    }

                }
                else{
                    //printf("echec d'analyse du non ter %s\n", ptr->value->atom.cod.code);
                    analyse = 0;
                }

            } else{
                //printf("Erreur dans l'analyse\n");
            }
            break;
        }
    }

    return analyse;
}

void GPLAction(int action){
    //printf("Action : %d\n", action);
    switch(action){
    case 1:{
        int prevTop = tableVar.top;
        int top = varToAddress(prevGplSymbol.value.ident, ENT);
        if(prevTop == top){
            printf("Error program - Multiple declaration of %s\n", prevGplSymbol.value.ident);
            exit(-1);
        }
        setOpToPcode("LDA", pcode.top + 1);
        setValueToPcode(top, pcode.top + 1);
        break;
    }
    case 2:
        setOpToPcode("LDC", pcode.top + 1);
        setValueToPcode(prevGplSymbol.value.integer, pcode.top + 1);
        setOpToPcode("AFF", pcode.top + 1);
        break;
    case 3:{
        int prevTop = tableVar.top;
        int top = varToAddress(prevGplSymbol.value.ident, DBL);
        if(prevTop == top){
            printf("Error program - Multiple declaration of %s\n", prevGplSymbol.value.dbl);
            exit(-1);
        }
        setOpToPcode("LDA", pcode.top + 1);
        setValueToPcode(top, pcode.top + 1);
        break;
    }
    case 4:
        setOpToPcode("LDC", pcode.top + 1);
        setValueDToPcode(prevGplSymbol.value.dbl, pcode.top + 1);
        setOpToPcode("AFF", pcode.top + 1);
        break;
    case 5:{
        int prevTop = tableVar.top;
        int top = varToAddress(prevGplSymbol.value.ident, ENT);
        if(prevTop == top){
            printf("Error program - Multiple declaration of %s\n", prevGplSymbol.value.ident);
            exit(-1);
        }
        setOpToPcode("LDA", pcode.top + 1);
        setValueToPcode(top, pcode.top + 1);
        break;
    }
    case 6:
        setOpToPcode("LDC", pcode.top + 1);
        if(strcmp(prevGplSymbol.value.ident, "TRUE") == 0)
            setValueToPcode(1, pcode.top + 1);
        else
            setValueToPcode(0, pcode.top + 1);

        setOpToPcode("AFF", pcode.top + 1);
        break;
    case 7:{

        Variable v;
        int ret = getVariable(prevGplSymbol.value.ident, &v);
        if(ret == -1){
            printf("Error program - Variable %s not declared\n", prevGplSymbol.value.ident);
            exit(-1);
        }
        setOpToPcode("LDA", pcode.top + 1);
        int pos = varToAddress(prevGplSymbol.value.ident, v.type);
        setValueToPcode(pos, pcode.top + 1);
        pushValueToTravail(pcode.top);
        break;
    }
    case 8:{

        int varIndex = popValueToTravail();
        int addressVar = pcode.value[varIndex].pValue.value;
        if(tableVar.vars[addressVar].type == ENT){
            setOpToPcode("LDV", pcode.top + 1);
            setValueToPcode(addressVar, pcode.top + 1);
            setOpToPcode("LDC", pcode.top + 1);
            setValueToPcode(1, pcode.top + 1);
            setOpToPcode("ADD", pcode.top + 1);
            setOpToPcode("AFF", pcode.top + 1);
        } else {
            printf("Error program - Operator ++ usable only with integers\n");
            exit(-1);
        }
        break;
    }
    case 9:{
        int varIndex = popValueToTravail();
        int addressVar = pcode.value[varIndex].pValue.value;
        if(tableVar.vars[addressVar].type == ENT){
            setOpToPcode("LDV", pcode.top + 1);
            setValueToPcode(addressVar, pcode.top + 1);
            setOpToPcode("LDC", pcode.top + 1);
            setValueToPcode(1, pcode.top + 1);
            setOpToPcode("SUB", pcode.top + 1);
            setOpToPcode("AFF", pcode.top + 1);
        } else {
            printf("Error program - Operator -- usable only with integers\n");
            exit(-1);
        }
        break;
    }
    case 10:{
        int varIndex = popValueToTravail();
        pushValueToTravail(varIndex);
        int addressVar = pcode.value[varIndex].pValue.value;
        if(tableVar.vars[addressVar].type == ENT){
            setOpToPcode("LDC", pcode.top + 1);
            setValueToPcode(prevGplSymbol.value.integer, pcode.top + 1);
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 11:{
        int varIndex = popValueToTravail();
        pushValueToTravail(varIndex);
        int addressVar = pcode.value[varIndex].pValue.value;
        if(tableVar.vars[addressVar].type == DBL){
            setOpToPcode("LDC", pcode.top + 1);
            setValueToPcode(prevGplSymbol.value.dbl, pcode.top + 1);
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 12:{
        int varIndex = popValueToTravail();
        pushValueToTravail(varIndex);
        int addressVar = pcode.value[varIndex].pValue.value;
        Variable v;
        int address = getVariable(prevGplSymbol.value.ident, &v);
        if(tableVar.vars[addressVar].type == v.type){
            setOpToPcode("LDV", pcode.top + 1);
            setValueToPcode(address, pcode.top + 1);
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 13:
        pushValueToTravail(ADD);
        break;
    case 14:
        pushValueToTravail(SUB);
        break;
    case 15:
        pushValueToTravail(MUL);
        break;
    case 16:
        pushValueToTravail(DIV);
        break;
    case 17:{
        int op = popValueToTravail();
        int varIndex = popValueToTravail();
        pushValueToTravail(varIndex);
        int addressVar = pcode.value[varIndex].pValue.value;
        if(tableVar.vars[addressVar].type == ENT){
            setOpToPcode("LDC", pcode.top + 1);
            setValueToPcode(prevGplSymbol.value.integer, pcode.top + 1);
            const char* optxt = getOpByInt(op);
            if(strcmp(optxt, "") != 0){
                setOpToPcode(optxt, pcode.top + 1);
                setOpToPcode("AFF", pcode.top + 1);
            }
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 18:{
        int op = popValueToTravail();
        int varIndex = popValueToTravail();
        pushValueToTravail(varIndex);
        int addressVar = pcode.value[varIndex].pValue.value;
        if(tableVar.vars[addressVar].type == DBL){
            setOpToPcode("LDC", pcode.top + 1);
            setValueToPcode(prevGplSymbol.value.dbl, pcode.top + 1);
            const char* optxt = getOpByInt(op);
            if(strcmp(optxt, "") != 0){
                setOpToPcode(optxt, pcode.top + 1);
                setOpToPcode("AFF", pcode.top + 1);
            }
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 19:{
        int op = popValueToTravail();
        int varIndex = popValueToTravail();
        pushValueToTravail(varIndex);
        int addressVar = pcode.value[varIndex].pValue.value;
        Variable v;
        int address = getVariable(prevGplSymbol.value.ident, &v);
        if(tableVar.vars[addressVar].type == v.type){
            setOpToPcode("LDV", pcode.top + 1);
            setValueToPcode(address, pcode.top + 1);
            const char* optxt = getOpByInt(op);
            if(strcmp(optxt, "") != 0){
                setOpToPcode(optxt, pcode.top + 1);
                setOpToPcode("AFF", pcode.top + 1);
            }
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 20:
        setOpToPcode("JIF", pcode.top + 1);
        pushValueToTravail(pcode.top + 1);
        pcode.top++;
        break;
    case 21:
        setOpToPcode("JMP", pcode.top + 1);
        setValueToPcode(pcode.top + 2, popValueToTravail());
        pcode.top++;
        break;
    case 22:{
        int val = popValueToTravail();
        setValueToPcode(pcode.top + 1, val);
        break;
    }
    case 23:
        setOpToPcode("OR", pcode.top + 1);
        break;
    case 24:
        setOpToPcode("AND", pcode.top + 1);
        break;
    case 25:
        setOpToPcode("LDC", pcode.top + 1);
        setValueToPcode(prevGplSymbol.value.integer, pcode.top + 1);
        break;
    case 26:
        setOpToPcode("LDC", pcode.top + 1);
        if(strcmp(prevGplSymbol.value.ident, "TRUE") == 0)
            setValueToPcode(1, pcode.top + 1);
        else
            setValueToPcode(0, pcode.top + 1);
        break;
    case 27:{
        Variable v;
        int address = getVariable(prevGplSymbol.value.ident, &v);
        if(v.type == ENT){
            setOpToPcode("LDV", pcode.top + 1);
            setValueToPcode(address, pcode.top + 1);
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 28:{
        setOpToPcode("LDC", pcode.top + 1);
        setValueToPcode(prevGplSymbol.value.integer, pcode.top + 1);
        int op = popValueToTravail();
        const char* optxt = getOpByInt(op);
        if(strcmp(optxt, "") != 0){
            setOpToPcode(optxt, pcode.top + 1);
        }
        break;
    }
    case 29:{
        setOpToPcode("LDC", pcode.top + 1);

        if(strcmp(prevGplSymbol.value.ident, "TRUE") == 0)
            setValueToPcode(1, pcode.top + 1);
        else
            setValueToPcode(0, pcode.top + 1);

        int op = popValueToTravail();
        const char* optxt = getOpByInt(op);
        if(strcmp(optxt, "") != 0){
            setOpToPcode(optxt, pcode.top + 1);
        }
        break;
    }
    case 30:{
        Variable v;
        int address = getVariable(prevGplSymbol.value.ident, &v);
        if(v.type == ENT){
            setOpToPcode("LDV", pcode.top + 1);
            setValueToPcode(address, pcode.top + 1);
            int op = popValueToTravail();
            const char* optxt = getOpByInt(op);
            if(strcmp(optxt, "") != 0){
                setOpToPcode(optxt, pcode.top + 1);
            }
        } else {
            printf("Error program - Does not respect type\n");
            exit(-1);
        }
        break;
    }
    case 31:
        pushValueToTravail(SUP);
        break;
    case 32:
        pushValueToTravail(SUPE);
        break;
    case 33:
        pushValueToTravail(INF);
        break;
    case 34:
        pushValueToTravail(INFE);
        break;
    case 35:
        pushValueToTravail(EG);
        break;
    case 36:
        pushValueToTravail(NEG);
        break;
    case 37:
        setOpToPcode("JIF", pcode.top + 1);
        pushValueToTravail(pcode.top + 1);
        pcode.top++;
        break;
    case 38:{
        int add = popValueToTravail();
        int addBool = popValueToTravail();
        setValueToPcode(pcode.top + 3, add);
        setOpToPcode("JMP", pcode.top + 1);
        setValueToPcode(addBool, pcode.top + 1);
        break;
    }
    case 39:
        setOpToPcode("NOT", pcode.top + 1);
        break;
    case 40:
        setOpToPcode("LDC", pcode.top + 1);
        setValueToPcode(prevGplSymbol.value.integer, pcode.top + 1);
        setOpToPcode("WR", pcode.top + 1);
        break;
    case 41:
        setOpToPcode("LDC", pcode.top + 1);
        setValueToPcode(prevGplSymbol.value.dbl, pcode.top + 1);
        setOpToPcode("WR", pcode.top + 1);
        break;
    case 42:{
        Variable v;
        int address = getVariable(prevGplSymbol.value.ident, &v);
        setOpToPcode("LDV", pcode.top + 1);
        setValueToPcode(address, pcode.top + 1);
        setOpToPcode("WR", pcode.top + 1);
        break;
    }
    case 43:{
        Variable v;
        int address = getVariable(prevGplSymbol.value.ident, &v);
        setOpToPcode("LDA", pcode.top + 1);
        setValueToPcode(address, pcode.top + 1);
        setOpToPcode("RD", pcode.top + 1);
        setOpToPcode("AFF", pcode.top + 1);
        break;
    }

    case 44:{
        pushValueToTravail(pcode.top + 1);
        break;
    }
    case 45:{
        popValueToTravail();
        break;
    }

    }
}

const char* getOpByInt(int op){
    switch(op){
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case MUL:
        return "MUL";
    case DIV:
        return "DIV";
    case SUP:
        return "SUP";
    case SUPE:
        return "SUPE";
    case INF:
        return "INF";
    case INFE:
        return "INFE";
    case EG:
        return "EG";
    case NEG:
        return "NEG";
    }
    return "";
}

int isGplSymbol(const char* gplRead){
    int i;
    for(i = 0; i < 100; i++){
        if(strcmp(languageSymbols[i], gplRead) == 0){
            return 1;
        }
    }
    return 0;
}

// 1 si c'est entier 2 si c'est double
int isNumber(const char* gplRead){
    int i;
    int pointFound = 0;
    int isNumber;
    for(i = 0; i < strlen(gplRead); i++){
        if(!isdigit(gplRead[i])){
            if(gplRead[i] == '.'){
                if(!pointFound){
                    pointFound = 1;
                    continue;
                } else {
                    return 0;
                }

            }
            return 0;
        }
    }
    if(pointFound) return 2;
    else return 1;
}

void initGPLSymbols(){
    int i;
    for(i = 0; i < 100; i++){
        strcpy(languageSymbols[i], "");
    }
    strcpy(languageSymbols[0], ">");
    strcpy(languageSymbols[1], ">=");
    strcpy(languageSymbols[2], "<=");
    strcpy(languageSymbols[3], "<");
    strcpy(languageSymbols[4], "==");
    strcpy(languageSymbols[5], "=");
    strcpy(languageSymbols[6], "!=");
    strcpy(languageSymbols[7], "&&");
    strcpy(languageSymbols[8], "||");
    strcpy(languageSymbols[9], "!");
    strcpy(languageSymbols[10], "+");
    strcpy(languageSymbols[11], "-");
    strcpy(languageSymbols[12], "/");
    strcpy(languageSymbols[13], "*");
    strcpy(languageSymbols[14], "++");
    strcpy(languageSymbols[15], "--");
    strcpy(languageSymbols[16], "(");
    strcpy(languageSymbols[17], ")");
    strcpy(languageSymbols[18], "{");
    strcpy(languageSymbols[19], "}");
    strcpy(languageSymbols[20], "(-)");

    initKeywords();
    pcode.top = -1;
    pTravail.top = 0;
    tableVar.top = -1;
}

int isKeyword(const char* expr){
    int i;
    for(i = 0; i < 100; i++){
        if(strcmp(keywords[i], expr) == 0) return 1;
    }
    return 0;
}
void initKeywords(){
    int i;
    for(i = 0; i < 100; i++){
        strcpy(keywords[i], "");
    }
    strcpy(keywords[0], "PROGRAM");
    strcpy(keywords[1], "END");
    strcpy(keywords[2], "main");
    strcpy(keywords[3], "procedure");
    strcpy(keywords[4], "double");
    strcpy(keywords[5], "int");
    strcpy(keywords[6], "bool");
    strcpy(keywords[7], "NULL");
    strcpy(keywords[8], "if");
    strcpy(keywords[9], "then");
    strcpy(keywords[10], "else");
    strcpy(keywords[11], "TRUE");
    strcpy(keywords[12], "FALSE");
    strcpy(keywords[13], "while");
    strcpy(keywords[14], "const");
    strcpy(keywords[15], "read");
    strcpy(keywords[16], "print");

}

void setValueToPcode(int value, int pos){
    pcode.value[pos].type = ENTIER;
    pcode.value[pos].pValue.value = value;
    if(pos >= pcode.top) pcode.top = pos;
}

void setValueDToPcode(double value, int pos){
    pcode.value[pos].type = DOUBLE;
    pcode.value[pos].pValue.valueD = value;
    if(pos >= pcode.top) pcode.top = pos;
}

void setOpToPcode(char* op, int pos){
    pcode.value[pos].type = OP;
    strcpy(pcode.value[pos].pValue.op, op);
    if(pos >= pcode.top) pcode.top = pos;
}

void pushValueToTravail(int value){
    pTravail.value[pTravail.top] = value;
    pTravail.top++;
}
int popValueToTravail(){
    pTravail.top--;
    return pTravail.value[pTravail.top];
}

void displayPCode(){
    int i;
    printf("Pcode : top %d\n", pcode.top);
    for(i = 0; i <= pcode.top; i++){
        if(pcode.value[i].type == OP)
            printf("%d - op : %s\n", i, pcode.value[i].pValue.op);
        if(pcode.value[i].type == ENTIER)
            printf("%d - contenu : %d\n", i, pcode.value[i].pValue.value);
        if(pcode.value[i].type == DOUBLE)
            printf("%d - contenu : %lf\n", i, pcode.value[i].pValue.valueD);
    }
}

int varToAddress(char* ident, int type){
    int i;
    for(i = 0; i <= tableVar.top; i++){
        if(strcmp(tableVar.vars[i].idents, ident) == 0){
            if(tableVar.vars[i].type != type) return -1;
            return i;
        }
    }
    tableVar.top++;
    strcpy(tableVar.vars[tableVar.top].idents, ident);
    tableVar.vars[tableVar.top].type = type;
    return tableVar.top;
}

void displayVarList(){
    int i;
    printf("Table des vars\n");
    for(i = 0; i < 200; i++){
        if(strcmp(tableVar.vars[i].idents, "") != 0){
            printf("%d - %s (%d)\n", i, tableVar.vars[i].idents, tableVar.vars[i].type);
        }
    }
}

void storeToLastSymbol(){
    strcpy(prevGplSymbol.strRead, gplSymbol.strRead);
    prevGplSymbol.type = gplSymbol.type;
    switch(gplSymbol.type){
    case IDENT:
        strcpy(prevGplSymbol.value.ident, gplSymbol.value.ident);
        break;
    case ENT:
        prevGplSymbol.value.integer = gplSymbol.value.integer;
        break;
    case DBL:
        prevGplSymbol.value.dbl = gplSymbol.value.dbl;
        break;
    case SYMBOL:
        strcpy(prevGplSymbol.value.symbol, gplSymbol.value.symbol);
        break;
    case KEYWORD:
        strcpy(prevGplSymbol.value.ident, gplSymbol.value.ident);
        break;
    }
}

int getVariable(char* ident, Variable* found){
    int i;
    for(i = 0; i < 200; i++){
        if(strcmp(tableVar.vars[i].idents, ident) == 0){
            *found = tableVar.vars[i];
            return i;
        }
    }
    return -1;
}
