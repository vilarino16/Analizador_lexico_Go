#include <stdio.h>

#include "analizador_sintactico.h"
#include "sistema_de_entrada.h"
#include "TS.h"

int main(int argc, char** argv) {
    //Inicialización de estructuras de datos e impresión de la tabla de símbolos
    inicializar_TS();
    imprimir_TS();
    inicializar_Sistema_Entrada(argc, argv);
    //Análisis
    printf("Inicializando analizador\n");
    iniciar_bucle();
    printf("Finalizando analizador\n");
    imprimir_TS();
    //Finalización y limpieza
    liberar_TS();
    return 0;
}