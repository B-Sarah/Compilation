.PHONY: clean
main: main.c grammaire.o analyse.o stack.o gpl.o interpreter.o
grammaire.o: grammaire.c grammaire.h
analyse.o: analyse.c analyse.h
stack.o: stack.c stack.h
gpl.o: gpl.c gpl.h
interpreter.o : interpreter.c interpreter.h
clean:
	@rm *.o
