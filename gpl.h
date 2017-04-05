#ifndef GPL_H
#define GPL_H

#include <ctype.h>

#include "analyse.h"

extern char program[1000];

typedef enum{
    IDENT, ENT, DBL, SYMBOL, KEYWORD
}GPLType;

typedef enum{
    VAL, OP
}PCodeType;

typedef union{
    char ident[200];
    int integer;
    double dbl;
    char symbol[5];
}GPLValue;

typedef struct{
    GPLValue value;
    GPLType type;
    char strRead[200];
}GPLSymbol;

typedef union{
    int value;
    char op[5];
}PCodeUnion;

typedef struct{
    PCodeUnion pValue;
    PCodeType type;
}PCodeValue;

typedef struct{
    PCodeValue value[5000];
    int top;
}PCode;

typedef struct {
    int value[5000];
    int top;
}PileT;

typedef struct{
    char idents[200];
    int type;
}Variable;

typedef struct {
    Variable vars[200];
    int top;
}TableVar;

extern PCode pcode;
extern PileT pTravail;
extern TableVar tableVar;

extern GPLSymbol gplSymbol;
extern GPLSymbol prevGplSymbol;

extern char languageSymbols[100][10];
extern char keywords[100][200];

void ScanGPL();
int AnalyseGPL(PTR* ptr);
void GPLAction(int action);

void DisplayGPLSymbol(GPLSymbol s);
void initGPLSymbols();

int isKeyword(const char* expr);
void initKeywords();

void addValueToPcode(int value);
void addOpToPcode(char* op);

void setValueToPcode(int value, int pos);
void setOpToPcode(char* op, int pos);

void pushValueToTravail(int value);
int popValueToTravail();

void displayPCode();

int varToAddress(char* ident, int type);
void displayVarList();

void storeToLastSymbol();

#endif // GPL_H
