#include "stack.h"

void Push(Stack* s, PTR* value){
    if(IsFull(*s))
        return;

    s->top = s->top +1;
    s->stackArray[s->top] = value;
}
PTR* Pop(Stack* s){
    if(IsEmpty(*s))
        return NULL;

    return s->stackArray[s->top--];



}

int IsEmpty(Stack s){
    return s.top < 0;
}

int IsFull(Stack s){
    return s.top >= 50;
}

void initStack(Stack* s){
    s->top = -1;
}
