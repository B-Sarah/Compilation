#ifndef ANALYSE_H
#define ANALYSE_H

#include "grammaire.h"
#include "stack.h"

extern Stack actionStack;
extern char expressionGPL[5000];

typedef struct{
    Atom atome;
    char chaine[200];
}ScanSymbol;


extern ScanSymbol symbol;

extern char nonTerArray[200][200];
extern char terArray[200][200];

void ScanG0();
int AnalyseG0(PTR* ptr);
int GetActionFromElement(const char* element);
void DisplaySymbol(ScanSymbol symbol);
void InitTerNonTerArray();
int IsTer(const char* element);
int IsNonTer(const char* element);
void G0Action(int action);

const char* SearchDICOT();
const char* SearchDICONT(int* indx);

#endif
