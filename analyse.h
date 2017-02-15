#ifndef ANALYSE_H
#define ANALYSE_H

#include "grammaire.h"

extern char expression[200];

typedef struct{
    Atom atome;
    char chaine[20];
}ScanSymbol;

void Scan();
int Analyse(PTR* ptr);
int GetActionFromElement(const char* element);
void DisplaySymbol(ScanSymbol symbol);
#endif
