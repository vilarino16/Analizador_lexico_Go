#ifndef LISTA_H
#define LISTA_H

#define L 15 //Tamaño de las listas
//Definición del tipo de elemento almacenado en la lista
typedef struct {
    int complexico;
    char* lexema;
} TIPOELEMENTO; //Estructura del componente léxico y el lexema

// Estructura que define una posición de la lista
typedef struct celda * TPOSICION;
//Estructura que define la lista
typedef struct l * TLISTA;

/**
 * Reserva memoria para una lista de datos con el tipo [TIPOELEMENTO].
 */
void crearLista(TLISTA *l);

/**
 * Destruye (libera la memoria reservada) la lista [l] y todos los elementos que almacena.
 */
void destruirLista(TLISTA *l);
/**
 * Recupera la primera posicion de la lista.
 */
TPOSICION primeroLista(TLISTA l);

/**
 * Recupera la posicion del fin de la lista.
 */
TPOSICION finLista(TLISTA l);

/**
 * Devuelve la posicion siguiente a [p] en la lista [l].
 */
TPOSICION siguienteLista(TLISTA l, TPOSICION p);

/**
 * Devuelve la posicion anterior a [p] en la lista [l].
 */
TPOSICION anteriorLista(TLISTA l, TPOSICION p);

/**
 * Recupera el elemento almacenado en la posicion [p] pasada por argumento.
 */
void recuperarElementoLista(TLISTA l, TPOSICION p, TIPOELEMENTO *e);

/**
 * Consulta la longitud de la lista [l].
 */
unsigned longitudLista(TLISTA l);

/**
 * Inserta el elemento [e] en la posicion siguiente a la posicion [p] de la lista [l].
 */
void insertarElementoLista(TLISTA *l, TPOSICION p, TIPOELEMENTO e);

/**
 * Suprime el elemento en posicion [p] de la lista [l].
 */
void suprimirElementoLista(TLISTA *l, TPOSICION p);

/**
 * Modifica el valor del elemento almacenado en la posicion [p] guardando el nuevo elemento [e].
 */
void modificarElementoLista(TLISTA *l, TPOSICION p, TIPOELEMENTO e);
#endif  //LISTA_H