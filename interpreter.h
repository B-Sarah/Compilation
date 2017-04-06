#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "gpl.h"

typedef enum{
    XINT, XDBL
}PilexType;

typedef union{
    int value;
    double valueD;
}PilexUnion;

typedef struct{
    PilexUnion pValue;
    PilexType type;
}PilexValue;

typedef struct{
    PilexValue value[5000];
    int co;
    int spx;
}Pilex;

extern Pilex pilex;

void setPilexValue(int pos, int value);
void setPilexValueD(int pos, double value);

void interpret(const char* op);

void displayVariables();

#endif // INTERPRETER_H
