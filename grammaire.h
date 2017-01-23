#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	char cod[10];
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

PTR* GenConc(PTR* p1, PTR* p2);
PTR* GenUnion(PTR* p1, PTR* p2);
PTR* GenStar(PTR* p1);
PTR* GenUn(PTR* p1);
PTR* GenAtom(const char* cod, int action, AtomType aType);

void printRepeatedChar(char c, int cnt);
const char* DisplayAtom(AtomType atom);
void __DisplayTree(PTR* ptr, int _count);
void DisplayTree(PTR* ptr);


int Allocate(Operation operation, PTR** ptr);

int DestroyPtr(PTR** ptr);
void DestroyArrayOfPtr();

void InitArrayOfPtr();


#endif
