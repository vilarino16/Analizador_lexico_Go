#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "errores.h"
#include "sistema_de_entrada.h"

#define TAM_BLOQUE 62 //Tamaño del bloque físico, que se dividirá a la mitad en dos bloques lógicos. De esta manera el tamaño de cada bloque lógico es TAM_BLOQUE/2

//Variables globales
FILE *fp;
int delantero=0, inicio=0, recargado=0, error_mostrado=0;
char bloque[TAM_BLOQUE+2]; //Se utiliza un bloque físico que se divide en dos bloques lógicos. La suma de dos posiciones son por los dos EOF que hay que añadir al finalizar el bloque
size_t elementos_leidos = 0; //Elementos leídos por fread

//Declaración de funciones auxiliares
void cargar_bloqueA();
int distancia_punteros();

//Se abre el archivo que se pasa por parámetros y se carga el primer bloque lógico en la variable bloque
void inicializar_Sistema_Entrada(int n_param, char **parametros) {
    if (n_param > 2 && !strcmp(parametros[1], "-f")) { //Formato del fichero como entrada del archivo: ejecutable.o -f concurrentSum.go
        if ((fp = fopen(parametros[2], "r")) == NULL) { //En el caso de no poder abrir el archivo, se emite un mensaje de error por pantalla y se detiene la ejecución
            emitir_error(0);
            exit(1);
        }
        //Se carga el primer bloque lógico
        cargar_bloqueA();
    }
}

//Función de carga del primer bloque lógico
void cargar_bloqueA() {
    elementos_leidos = fread(bloque, 1, TAM_BLOQUE/2, fp); //Se cargan TAM_BLOQUE/2 caracteres en la primera mitad del bloque físico, siendo el primer bloque lógico
    bloque[elementos_leidos]=EOF; //Se añade un EOF al final del buffer de caracteres leídos, en el caso de que se termine el archivo se añade el EOF del archivo que fread haciendo uso de la variable de los elementos leídos
}

//Función de carga del segundo bloque lógico
void cargar_bloqueB() {
    elementos_leidos = fread(bloque+1+TAM_BLOQUE/2, 1, TAM_BLOQUE/2, fp); //Se cargan los caracteres del segundo bloque lógico después del EOF que marca el final del primer bloque
    bloque[(TAM_BLOQUE/2)+1+elementos_leidos]=EOF; //De forma similar a la función de cargar_bloqueA se añade un EOF al final del segundo bloque lógico o al final del archivo de entrada
}

//Lectura del siguiente carácter del fichero, delantero siempre tendrá un valor superior por una unidad a la posición del carácter devuelto
int sig_caracter() {
    //Comprobación de que el tamaño del lexema a recuperar no supera el tamaño máximo seguro que el sistema de entrada puede soportar
    //En el caso de superar el tamaño máximo de lexema, se avanza el puntero inicio al comienzo del siguiente bloque
    if (distancia_punteros() > TAM_BLOQUE/2) {
        if (!error_mostrado) {//Se evita que el mismo error se muestre repetidas veces para un mismo lexema
            emitir_error(1);
            error_mostrado=1;
        }
        if (inicio <= TAM_BLOQUE/2) {
            inicio=TAM_BLOQUE/2+1;
        }else if (inicio >= TAM_BLOQUE/2+1) {
            inicio=0;
        }
    }
    if (bloque[delantero]==EOF) { //Condición necesaria en el método del centinela para cambiar de bloque o finalizar la lectura del archivo
        if (delantero==TAM_BLOQUE/2) { //Paso al segundo bloque lógico
            delantero+=2; //Se suma un valor adicional por la lógica del programa, delantero siempre está adelantado 1 posición
            if (!recargado) { //En caso de algún retroceso, no se puede volver a cargar otro bloque de datos porque se perdería el bloque anterior
                cargar_bloqueB();
            }
            recargado=0; //Sirve para marcar que se ha cargado un nuevo bloque de datos
            return bloque[delantero-1];
        }
        if (delantero==TAM_BLOQUE+1) { //Paso al primer bloque lógico, proceso similar al de la carga del bloqueB
            delantero=1;
            if (!recargado) {
                cargar_bloqueA();
            }
            recargado=0;
            return bloque[delantero-1];
        }
        if (elementos_leidos < TAM_BLOQUE/2) { // Si se ha terminado de leer el archivo, se devuelve el EOF
            return EOF;
        }
    }
    //Ejecución habitual de la función, se devuelve el siguiente carácter
    //La resta se debe a que la implementación está realizada teniendo en cuenta que delantero va 1 posición adelantada al carácter que se devuelve
    delantero++;
    return bloque[delantero-1];
}

//Función para retroceder el puntero inicio, se utiliza según el comportamiento de algunos autómatas
void retroceder_inicio() {
    inicio--;
    if (inicio<0) { //Los valores siempre se deben mantener en el rango de los bloques lógicos
        inicio=TAM_BLOQUE;
    } else if (inicio==TAM_BLOQUE/2) {
        inicio--;
    }
}

//Función para actualizar inicio en casos puntuales
void actualizar_inicio() {
    inicio=delantero;
}

//Función para retroceder el puntero delantero según las necesidades de los autómatas
void retroceder_delantero() {
    delantero--;
    if (delantero<0) { //Los valores del puntero se deben mantener en el rango de posiciones de los bloques
        recargado=1; //Se utiliza para prevenir que se sobrescriba algún bloque de datos
        delantero=TAM_BLOQUE;
    }else if (delantero==TAM_BLOQUE/2) {
        recargado=1;
        delantero--;
    }
}

//Función para recuperar el lexema analizado por el analizador léxico
char* recuperar_lexema() {
    error_mostrado=0; //Se reinicia para mostrar el siguiente error diferente
    char* lexema= malloc(sizeof(char)* distancia_punteros()+1); //Se añade la posición del fin de cadena '\0'
    int cont=0; //Contador del tamaño del lexema para usarlo como índice de la cadena a construir
    while (delantero-1!=inicio) { //El valor de delantero está adelantado, por eso se realiza hasta delantero-1
        if (bloque[inicio]==EOF) { //Se cubren los casos de saltos entre bloques
            if (inicio==TAM_BLOQUE+1) {
                inicio=0;
                continue;
            }
            inicio++;
            continue;
        }
        lexema[cont]=bloque[inicio]; //Se guarda el carácter en la cadena a devolver
        cont++;
        inicio++;
    }
    //Se guarda el fin de cadena y se equilibran los punteros de inicio y delantero
    inicio++;
    lexema[cont]='\0';
    return lexema;
}

//Función para cerrar el archivo
void cerrar_entrada() {
    fclose(fp);
}

//Función auxiliar para calcular la distancia entre los punteros en el ciclo de valores
int distancia_punteros() {
    if (delantero>inicio) {
        if (delantero >= TAM_BLOQUE/2+1 && inicio <= TAM_BLOQUE/2) { //Se tiene en cuenta la posición del primer bloque lógico para un cálculo correcto
            return delantero-1-inicio;
        }
        return delantero-inicio;
    }if (delantero<inicio) {
        return (delantero+TAM_BLOQUE+1)-inicio;
    }
    return 0;
}