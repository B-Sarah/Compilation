#include "grammaire.h"
#include "gpl.h"
#include "interpreter.h"

/*char expressionGPL[5000]= "\
S -> [ N . '->' . E . ',' ] . ';' , \
N -> 'IDNTER' , \
E -> T . [ '+' . T ] , \
T -> F . [ '.' . F ] , \
F -> 'IDNTER' + 'ELETER' + '(' . E . ')' + '[' . E . ']' + '[|' . E . '|]' , ;";*/

/*char expressionGPL[5000]="\
S -> [ 'a' ] . b , ;";*/

/*char expressionGPL[5000]="\
Program -> 'Program' . identif . [| decl_const |] . [| decl_var |] . [| decl_tab |] . [| decl_proc |] . block . '.' , \
decl_const -> 'const' . identif . '=' . number . ';' . [ identif . '=' . number . ';' ] , \
number -> '-' . [ ent ] + [ ent ] + '-' . double + '-' . double , \
ent -> '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' , \
double -> [ ent ] . '.' . ent . [ ent ] , \
";*/

char expressionGPL[5000]="\
program -> 'PROGRAM' . ident . [ proc ] . main . 'END' . ident , \
main -> 'main' . bloc , \
proc -> 'procedure' . ident . '(' . param . ')' . bloc , \
\
param -> ( 'int' . ident ) + ( 'double' . ident ) + ( 'bool' . ident ) + 'NULL' , \
bloc -> '{' . [ inst ] . '}' , \
inst -> if + varinit + constaff + varaff + read + write + while , \
\
if -> 'if' . '(' . exprBool#20 . ')' . '{' . [ inst ] . '}' . else#22 , \
else -> [| 'else'#21 . '{' . [ inst ] . '}' |] , \
\
while -> 'while' . '('#44 . exprBool#37 . ')' . '{' . [ inst ] . '}'#38 , \
\
write -> 'print' . '(' . ( int#40 + double#41 + ident#42 ) . ')' . ';' , \
read -> 'read' . '(' . ident#43 . ')' . ';' , \
\
constaff -> 'const' . varinit , \
varinit -> boolinit + intinit + doubleinit , \
intinit -> 'int' . ident#1 . '=' . int#2 . ';' , \
boolinit -> 'bool' . ident#5 . '=' . bool#6 . ';' , \
doubleinit -> 'double' . ident#3 . '=' . double#4 . ';' , \
varaff -> ident#7 . ( ( '=' . exprArithm#45 ) + '++'#8 + '--'#9 ) . ';' , \
exprArithm -> ( int#10 + double#11 + ident#12 ) . [ ( '+'#13 + '-'#14 + '*'#15 + '/'#16 ) . ( int#17 + double#18 + ident#19 ) ] , \
\
int -> [| '(-)' |] . digit . [ digit ] , \
double -> [| '(-)' |] . [ digit ] . '.' . digit . [ digit ] , \
\
digit -> '0' + '1' + '2' + '3' + '4' + '5' + '6' + '7' + '8' + '9' , \
ident -> string , \
string -> letter . [ letter + digit ] , \
letter -> 'a' + 'b' + 'c' + 'd' + 'e' + 'f' + 'g' + 'h' + 'i' + 'j' + 'k' + 'l' + 'm' + 'n' + 'o' + 'p' + 'q' + 'r' + 's' + 't' + 'u' + 'v' + 'w' + 'x' + 'y' + 'z' , \
\
exprBool -> TB . [ '||' . TB#23 ] , \
TB -> FB . [ '&&' . FB#24 ] , \
FB -> PB + '!' . PB#39 , \
PB -> ( int#25 + bool#26 + ident#27 ) . [ oprel . ( int#28 + bool#29 + ident#30 ) ] , \
oprel -> '>'#31 + '>='#32 + '<'#33 + '<='#34 + '=='#35 + '!='#36 , \
bool -> 'TRUE' + 'FALSE' , ;";


char program[1000] = "\
PROGRAM monprog \
main { \
int i = 8 ; \
int j = 3 ; \
while ( j < i ) { \
i ++ ; \
if ( i > 5 ) { \
j = j + 2 ; \
} \
} \
} \
END monprog";

/*
procedure maproc ( int valeur ) { \
} \
procedure maproc ( NULL ) { \
} \
const double valeur = 54.5 ; \
const bool isrunning = FALSE ; \
x = 40.50 ; \
y = TRUE ; \
y ++ ; \
isrunning = ! i <= 50.5 && ! TRUE ; \
if ( i < 8 ) { \
i ++ ; \
} \
else { \
j ++ ; \
print ( 5 ) ; \
read ( i ) ; \
} \
\
while ( i < 3 && i > 0 || FALSE ) { \
\
} \
*/

char nonTerArray[200][200];
char terArray[200][200];

int main(){
    int i;

    InitArrayOfPtr();
    InitTerNonTerArray();
    initGPLSymbols();


    for(i=0; i<5; i++){
     DisplayTree(treeArray[i]);
     printf("\n");
    }

    ScanG0();

    printf("ANALYSE G0\n");
    if(AnalyseG0(treeArray[S]))
        printf("OK\n");
    else
        printf("Pas OK\n");

    printf("\nNonter array : \n ");
    for(i = 0; i< 200; i++){
        if(strcmp(nonTerArray[i], "") != 0)
            printf("index %d : %s\n ", i, nonTerArray[i]);
    }

    printf("\n");

    for(i=5; i<50; i++){
        if(treeArray[i] != NULL){
            DisplayTree(treeArray[i]);
            printf("\n");
        }
    }

    /*for(i = 0; i < 200; i++){
        if(strcmp(terArray[i], "") != 0)
            printf("ter : %s\n", terArray[i]);
    }*/

    ScanGPL();

    printf("\nANALYSE GPL\n");
    if(AnalyseGPL(treeArray[5]))
        printf("GPL OK\n");
    else{
        printf("GPL Pas OK\n");
        exit(-1);
    }

    printf("\n");

    displayPCode();
    printf("\n");
    displayVarList();
    printf("\n");

    printf("INTERPRET\n");

    initPilex();
    interpretAll();

    printf("\nETAT variables\n");
    displayVariables();

    DestroyArrayOfPtr();

	return 0;
}
