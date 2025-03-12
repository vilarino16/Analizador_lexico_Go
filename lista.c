
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "definiciones.h"
#include "lista.h"

struct celda {
    TIPOELEMENTO elemento;
    struct celda *sig;
};

struct l {
    TPOSICION inicio;
    unsigned longitud;
    TPOSICION fin;
};

//Creación de la lista
void crearLista(TLISTA *l) {
    (*l) = (TLISTA) malloc(sizeof (struct l));
    (*l)->inicio = (TPOSICION) malloc(sizeof (struct celda));
    (*l)->inicio->elemento.complexico = 0;
    (*l)->fin = (*l)->inicio;
    ((*l)->fin)->sig = NULL;
    (*l)->longitud = 0;
}

//Liberación de la memoria y destrucción de la lista
void destruirLista(TLISTA *l) {
    (*l)->fin = (*l)->inicio;
    while ((*l)->fin != NULL) {
        (*l)->fin = ((*l)->fin)->sig;
        if ((*l)->inicio != NULL) {
            if (&((*l)->inicio->elemento) != NULL) {
                if (&((*l)->inicio->elemento.complexico) != NULL){
                    if ((*l)->inicio->elemento.complexico == ID) { //En el caso de los identificadores es necesario liberar la memoria reservada dinámicamente en la variable del lexema
                        free((*l)->inicio->elemento.lexema);
                    }
                }
            }
        }
        free((*l)->inicio);
        (*l)->inicio = (*l)->fin;
    }
    free(*l);
    *l = NULL;
}

unsigned esListaVacia(TLISTA l) {
    if (l->longitud == 0) return 1;
    return 0;
}

TPOSICION primeroLista(TLISTA l) {
    return (l->inicio);
}

TPOSICION siguienteLista(TLISTA l, TPOSICION p) {
    return (p->sig);
}

TPOSICION finLista(TLISTA l) {
    return (l->fin);
}

TPOSICION anteriorLista(TLISTA l, TPOSICION p) {
    TPOSICION q;
    q = l->inicio;
    while (q->sig != p) {
        q = q->sig;
    }
    return q;
}

void recuperarElementoLista(TLISTA l, TPOSICION p, TIPOELEMENTO *e) {
    *e = (p->sig)->elemento;
}

unsigned longitudLista(TLISTA l) {
    return (l->longitud);
}

void insertarElementoLista(TLISTA *l, TPOSICION p, TIPOELEMENTO e) {
    TPOSICION q;
    q = p->sig;
    TPOSICION nuevo = malloc(sizeof(struct celda));
    nuevo->elemento.lexema = e.lexema;
    nuevo->elemento.complexico = e.complexico;
    nuevo->sig = q;
    p->sig = nuevo;
    if (q == NULL) {
        (*l)->fin = nuevo;
    }
    (*l)->longitud = (*l)->longitud + 1;
}

void suprimirElementoLista(TLISTA *l, TPOSICION p) {
    TPOSICION q;
    q = p->sig;
    p->sig = q->sig;
    if (p->sig == NULL) (*l)->fin = p;
    free(q);
    (*l)->longitud = (*l)->longitud - 1;
}

void modificarElementoLista(TLISTA *l, TPOSICION p, TIPOELEMENTO e) {
    (p->sig)->elemento = e;
}





