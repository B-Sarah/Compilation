.PHONY: clean
main: main.c grammaire.o
grammaire.o: grammaire.c grammaire.h
clean:
	@rm *.o
