
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "analizador_sintactico.h"
#include "analizador_lexico.h"
#include "definiciones.h"
#include "lista.h"

//Variables globales
int copia=0; //Variable para identificadores repetidos de los cuales se puede liberar la memoria

//Función del análisis sintáctico donde se imprimen la pareja de componente léxico y lexema
void iniciar_bucle() {
    TIPOELEMENTO comp_lexema = sig_complex(); //Se pide el primer componente léxico junto con su lexema
    while (comp_lexema.complexico != EOF) { //Hasta que no se reciba el fin del archivo no se termina el análisis
        printf("<%d,%s>\n", comp_lexema.complexico, comp_lexema.lexema);
        if (comp_lexema.complexico != ID  || copia) { //Se libera memoria que no dañe la ejecución del programa, es decir la que no se necesita en la tabla de símbolos que provocaría problemas en la ejecución del programa
            free(comp_lexema.lexema);
            comp_lexema.lexema = NULL;
            copia = 0;
        }
        comp_lexema = sig_complex(); //Se pide el siguiente componente léxico
    }
    free(comp_lexema.lexema); //Se libera el lexema de EOF
    cerrar_analizador(); //Cierra el analizador léxico
}

//Función para que la TS pueda avisar de que es un identificador repetido y se pueda liberar la memoria
void repetido() {
    copia=1;
}
