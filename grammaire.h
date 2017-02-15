#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define S 0

union __PTR;
typedef union __PTR __PTR;

struct PTR;
typedef struct PTR PTR;

typedef enum {TER,NONTER} AtomType;
typedef enum {CONC,UNION,STAR,UN,ATOM} Operation;


typedef struct {
	PTR* left;
	PTR* right;
} Conc;

typedef struct {
	PTR* left;
	PTR* right;
} Union;

typedef struct {
	PTR* stare;
} Star;

typedef struct {
	PTR* une;
} Un;

typedef struct {
	char code[10];
	int index;
} Cod;

typedef struct {
    Cod cod;
	int action;
	AtomType aType;
} Atom;

union __PTR{
	Conc conc;
	Union _union;
	Star star;
	Un un;
	Atom atom;
};


struct PTR{
    __PTR* value;
    Operation type;
};

extern PTR* treeArray[5];
extern const char* g0NonTerArray[5];
extern const char* nonTerArray[200];

PTR* GenConc(PTR* p1, PTR* p2);
PTR* GenUnion(PTR* p1, PTR* p2);
PTR* GenStar(PTR* p1);
PTR* GenUn(PTR* p1);
PTR* GenAtom(const char* cod, int action, AtomType aType);

int elementAlreadyRead(const char* element);
int elementMustBeAdded(const char* element);

void printRepeatedChar(char c, int cnt);
const char* DisplayAtom(AtomType atom);
void __DisplayTree(PTR* ptr, int _count);
void DisplayTree(PTR* ptr);


int Allocate(Operation operation, PTR** ptr);

int DestroyPtr(PTR** ptr);
void DestroyArrayOfPtr();

void InitArrayOfPtr();
void initNonTerArray();
int IsNonTer(const char* element);


#endif
