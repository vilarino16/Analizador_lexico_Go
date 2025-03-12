
#ifndef ANALIZADOR_SINTACTICO_H
#define ANALIZADOR_SINTACTICO_H

//Función para iniciar el bucle de productor-consumidor del análisis, donde se van imprimiendo las pareja de componente léxico y lexema
void iniciar_bucle();

//Función para que la TS pueda avisar de que es un identificador repetido y se pueda liberar la memoria
void repetido();

#endif //ANALIZADOR_SINTACTICO_H
