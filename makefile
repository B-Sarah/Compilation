.PHONY: clean
main: main.c grammaire.o analyse.o
grammaire.o: grammaire.c grammaire.h
analyse.o: analyse.c analyse.h
clean:
	@rm *.o
