#include "grammaire.h"
PTR[5] treeArray;

PTR* GenConc(PTR p1, PTR p2){
	PTR* p;
	p->conc->left = p1;
	p->conc->right = p2;

	return p;
}
PTR* GenUnion(PTR p1, PTR p2){
	PTR* p;
	p->_union->left = p1;
	p->_union->right = p2;

	return p;
}
PTR* GenStar(PTR p1){
	PTR* p;
	p->star->stare = p1;

	return p;
}
PTR* GenUn(PTR p1){
	PTR* p;
	p->un->une = p1;

	return p;
}
PTR* GenAtom(char cod, int action, AtomType aType){
	PTR* p;
	p->atom->cod = cod;
	p->atom->action = action;
	p->atom->aType = aType;

	return p;
}
