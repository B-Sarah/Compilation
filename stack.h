#ifndef STACK_H
#define STACK_H

#include "grammaire.h"

typedef struct{
    int top;
    PTR* stackArray[50];
}Stack;

void Push(Stack* s, PTR* value);
PTR* Pop(Stack* s);
int IsEmpty(Stack s);
int IsFull(Stack s);
void initStack(Stack* s);

#endif

