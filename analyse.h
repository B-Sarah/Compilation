#ifndef ANALYSE_H
#define ANALYSE_H

#include "grammaire.h"
#include "stack.h"

extern Stack actionStack;
extern char expression[200];

typedef struct{
    Atom atome;
    char chaine[20];
}ScanSymbol;


extern ScanSymbol symbol;

extern const char* nonTerArray[200];
extern const char* terArray[200];

void Scan();
int Analyse(PTR* ptr);
int GetActionFromElement(const char* element);
void DisplaySymbol(ScanSymbol symbol);
void InitTerNonTerArray();
int IsTer(const char* element);
int IsNonTer(const char* element);

const char* Search(const char* DICO[]);

#endif
