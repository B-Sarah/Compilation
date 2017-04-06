#include "interpreter.h"

Pilex pilex;

void initPilex(){
    pilex.co = 0;
    pilex.spx = -1;
}

void setPilexValue(int pos, int value){
    pilex.value[pos].pValue.value = value;
    pilex.value[pos].type = XINT;
}
void setPilexValueD(int pos, double value){
    pilex.value[pos].pValue.valueD = value;
    pilex.value[pos].type = XDBL;
}

void interpretAll(){

    pilex.spx = tableVar.top;


    while(pilex.co <= pcode.top){
        //printf("co : %d\n", pilex.co);
        //getchar();
        interpret(pcode.value[pilex.co].pValue.op);
    }
}

void interpret(const char* op){
    if(strcmp(op, "LDA") == 0){
        pilex.spx++;
        pilex.value[pilex.spx].pValue.value = pcode.value[pilex.co + 1].pValue.value;
        pilex.co += 2;
    }
    if(strcmp(op, "LDC") == 0){
        pilex.spx++;
        //if(pcode.value[pilex.co + 1].type == ENTIER)
            setPilexValue(pilex.spx, pcode.value[pilex.co + 1].pValue.value);
        //else
            //setPilexValueD(pilex.spx, pcode.value[pilex.co + 1].pValue.valueD);
        pilex.co += 2;
    }
    if(strcmp(op, "LDV") == 0){
        pilex.spx++;
        //if(pilex.value[pcode.value[pilex.co + 1].pValue.value].type == XINT)
            setPilexValue(pilex.spx, pilex.value[pcode.value[pilex.co + 1].pValue.value].pValue.value);
        //else
            //setPilexValueD(pilex.spx, pilex.value[pcode.value[pilex.co + 1].pValue.value].pValue.valueD);
        pilex.co += 2;
    }
    if(strcmp(op, "JMP") == 0){
        pilex.co = pcode.value[pilex.co + 1].pValue.value;
    }
    if(strcmp(op, "JIF") == 0){
        if(pilex.value[pilex.spx].pValue.value == 0)
            pilex.co = pcode.value[pilex.co + 1].pValue.value;
        else pilex.co+=2;
    }
    if(strcmp(op, "ADD") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value + pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "SUB") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value - pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "MUL") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value * pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "DIV") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value / pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "INFE") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value <= pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "INF") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value < pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "SUP") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value > pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "SUPE") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value >= pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "EG") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value == pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "NEG") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value != pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "NOT") == 0){
        pilex.value[pilex.spx].pValue.value = ! pilex.value[pilex.spx].pValue.value;
        pilex.co++;
    }
    if(strcmp(op, "OR") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value || pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "AND") == 0){
        pilex.value[pilex.spx - 1].pValue.value = pilex.value[pilex.spx - 1].pValue.value && pilex.value[pilex.spx].pValue.value;
        pilex.spx-- ;
        pilex.co++;
    }
    if(strcmp(op, "RD") == 0){
        pilex.spx++;
        scanf("%d", &pilex.value[pilex.spx].pValue.value);
        pilex.co++;
    }
    if(strcmp(op, "WR") == 0){
        printf("%d\n", pilex.value[pilex.spx].pValue.value);
        pilex.co++;
    }
    if(strcmp(op, "AFF") == 0){
        pilex.value[pilex.value[pilex.spx - 1].pValue.value].pValue.value = pilex.value[pilex.spx].pValue.value;
        pilex.co++;
        pilex.spx -= 2;
    }
}

void displayVariables(){
    int i;
    for(i = 0; i <= tableVar.top; i++){
        printf("%s : %d\n", tableVar.vars[i].idents, pilex.value[i].pValue.value);
    }
}
