
#include "errores.h"
#include <stdio.h>

//Función de impresión de los errores
void emitir_error(int error) {
    switch (error) {
        case 0:
            printf("\033[31mError de entrada: No se ha logrado abrir el archivo\033[0m\n");
            break;
        case 1:
            printf("\033[31mError de entrada: Lexema demasiado grande\033[0m\n");
            break;
        case 2:
            printf("\033[31mError léxico: Comentario no finalizado\033[0m\n");
            break;
        case 3:
            printf("\033[31mError léxico: Doble exponente\033[0m\n");
            break;
        case 4:
            printf("\033[31mError léxico: Barra baja mal utilizada en los números\033[0m\n");
            break;
        case 5:
            printf("\033[31mError léxico: String interpretado terminado abruptamente\033[0m\n");
            break;
        default:
            break;
    }
}
