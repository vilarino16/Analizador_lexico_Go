#ifndef TABLA_HASH_ENCADENAMIENTO_H
#define TABLA_HASH_ENCADENAMIENTO_H

/* 
 * Tabla hash con encadenamiento
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"

#define N 97 //Número de entradas de la tabla

typedef TLISTA TablaHash[N];

/**
 * Inicializa cada elemento de la tabla a una lista vacía
 */
void Inicializar(TablaHash t);

/**
 * Destruye la lista que corresponde a cada elemento de la tabla
 */
void Destruir(TablaHash t);

/**
 * Función para encontrar la posicion de una entrada en la tabla
 */
int FuncionHash(char *cad);

/**
 * Buscar una clave en la tabla hash
 */
int Buscar(TablaHash t, char *clavebuscar);

/**
 * Inserta un elemento en una tabla hash al principio de la lista
 */
void Insertar(TablaHash *t, TIPOELEMENTO elemento);

/**
 * Borra un elemento en una tabla hash
*/
void Borrar(TablaHash *t, char *claveborrar);

/*
 * Imprime el estado de la tabla hash
 */
void Imprimir(TablaHash t);

#endif	// TABLA_HASH_ENCADENAMIENTO_H

