
#ifndef TS_H
#define TS_H

//Función para buscar un lexema en la tabla, si no se encuentra en la tabla se inserta como identificador
int buscar(char *lexema);

//Función de inicialización, se insertan todas las palabras reservadas en la tabla hash
void inicializar_TS();

//Inserción de los lexemas que se encuentran en el código como identificadores, ya que es la única opción
void insertar(char* complex);

//Elimina un elemento de la tabla hash, no se utiliza en esta práctica al considerar un solo nivel de scope o nivel de bloque léxico
void eliminar(char* complex);

//Liberación de la memoria de la tabla hash
void liberar_TS();

//Función para imprimir la tabla hash
void imprimir_TS();

#endif //TS_H
