#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

/*struct Conc;
typedef struct Conc Conc;
struct Union;
typedef struct Union Union;
struct Star;
typedef struct Star Star;
struct Un;
typedef struct Un Un;
struct Atom;
typedef struct Atom Atom;
union PTR;
typedef union PTR PTR;

extern PTR[5] treeArray;

typedef enum {TER,NONTER} AtomType;

typedef union {
	Conc conc;
	Union _union;
	Star star;
	Un un;
	Atom atom;
} PTR;
 
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
	char cod;
	int action;
	AtomType aType;
} Atom;
*/

PTR* GenCon(PTR p1, PTR p2);
PTR* GenUnion(PTR p1, PTR p2);
PTR* GenStar(PTR p1);
PTR* GenUn(PTR p1);
PTR* GenAtom(char cod, int action, AtomType aType);


#endif
