
#include "TS.h"
#include "analizador_sintactico.h"
#include "definiciones.h"
#include "tabla_hash_encadenamiento.h"

#define N_RESERVADAS 25

//Variable de la tabla hash
TablaHash t;

//Función para buscar un lexema en la tabla, si no se encuentra en la tabla se inserta como identificador
int buscar(char* lexema) {
    int a = Buscar(t, lexema);
    if(a) {
        if (a==ID) { //Se avisa al analizador sintáctico de que el lexema está repetido y así pueda liberar la memoria
            repetido();
        }
        return a;
    }
    insertar(lexema);
    return ID;
}

//Función de inicialización, se insertan todas las palabras reservadas en la tabla hash
void inicializar_TS() {
    TIPOELEMENTO palabras_reservados[N_RESERVADAS] = {{PACKAGE,"package"}, {BREAK, "break"}, {DEFAULT, "default"},{FUNC,"func"}, {INTERFACE,"interface"}, {SELECT, "select"},
        {CASE, "case"}, {DEFER, "defer"}, {GO, "go"}, {MAP,"map"},{STRUCT, "struct"},
        {CHAN, "chan"}, {ELSE, "else"}, {GOTO, "goto"}, {SWITCH, "switch"},
        {CONST, "const"}, {FALLTHROUGH, "fallthrough"}, {IF, "if"}, {RANGE, "range"}, {TYPE, "type"},
        {CONTINUE, "continue"}, {FOR, "for"}, {IMPORT, "import"}, {RETURN, "return"}, {VAR, "var"} };
    Inicializar(t);
    for (int i = 0; i < sizeof(palabras_reservados)/sizeof(TIPOELEMENTO); i++) {
        Insertar(&t, palabras_reservados[i]);
    }
}

//Función para imprimir la tabla hash
void imprimir_TS() {
    Imprimir(t);
}

//Liberación de la memoria de la tabla hash
void liberar_TS() {
    Destruir(t);
}

//Inserción de los lexemas que se encuentran en el código como identificadores, ya que es la única opción
void insertar(char* complex) {
    TIPOELEMENTO e;
    e.complexico = ID;
    e.lexema = complex;
    Insertar(&t, e);
}

//Elimina un elemento de la tabla hash, no se utiliza en esta práctica al considerar un solo nivel de scope o nivel de bloque léxico
void eliminar(char* complex) {
    Borrar(&t, complex);
}