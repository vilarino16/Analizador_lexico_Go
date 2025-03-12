
#include "tabla_hash_encadenamiento.h"

/* TABLA HASH CON ENCADENAMIENTO */

/******* FUNCIONES HASH *******/
int FuncionHash(char *cad) {
    int suma = 0, posicion = 0;
    //Suma ponderada de los valores ASCII (K=256)
    for (int i = (int)strlen(cad) - 1; i >= 0; i--)
        suma = (suma * 256 + cad[i]) % N;
    posicion = suma;
    return posicion;
}

void Insertar(TablaHash *t, TIPOELEMENTO elemento) {
    int pos = FuncionHash(elemento.lexema);
    insertarElementoLista(&(*t)[pos], primeroLista((*t)[pos]), elemento);
}


int Buscar(TablaHash t, char *clavebuscar) {
    TPOSICION p;
    unsigned int encontrado = 0;
    TIPOELEMENTO ele;

    int pos = FuncionHash(clavebuscar);

    p = primeroLista(t[pos]);
    while (p != finLista(t[pos]) && !encontrado) {
        recuperarElementoLista(t[pos], p, &ele);
        if (strcmp(ele.lexema, clavebuscar) == 0) {
            encontrado = ele.complexico;
        } else {
            p = siguienteLista(t[pos], p);
        }
    }
    return (int) encontrado;
}

/* Elimina de la tabla el elemento con la clave indicada */
void Borrar(TablaHash *t, char *claveborrar) {
    TPOSICION p;
    TIPOELEMENTO elemento;
    int pos = FuncionHash(claveborrar);

    p = primeroLista((*t)[pos]);
    recuperarElementoLista((*t)[pos], p, &elemento);
    while (p != finLista((*t)[pos]) && strcmp(claveborrar, elemento.lexema)) {
        p = siguienteLista((*t)[pos], p);
        recuperarElementoLista((*t)[pos], p, &elemento);
    }
    if (p != finLista((*t)[pos]))
        suprimirElementoLista(&(*t)[pos], p);
}

/* CREAR TABLA VACIA */
void Inicializar(TablaHash t) {
    for (int i = 0; i < N; i++) {
        crearLista(&t[i]);
    }
}

/* DESTRUIR TABLA */
void Destruir(TablaHash t) {
    for (int i = 0; i < N; i++)
        destruirLista(&t[i]);
}

/* IMPRIMIR TABLA */
void Imprimir(TablaHash t) {
    int cont =0;
    printf("---------------------Imprimiendo TS---------------------\n");
    TPOSICION p;
    TIPOELEMENTO elemento;
    for (int i = 0; i < N; i++) {
        p = primeroLista((t)[i]);
        if (p == finLista((t)[i])) {
            printf("Vacío\n");
            continue;
        }
        recuperarElementoLista((t)[i], p, &elemento);
        printf("%s", elemento.lexema);
        p = siguienteLista((t)[i], p);
        while (p != finLista((t)[i])) {
            if (p == finLista((t)[i])) {
                continue;
            }
            recuperarElementoLista((t)[i], p, &elemento);
            printf(" -> %s", elemento.lexema);
            cont++;
            p = siguienteLista((t)[i], p);
        }
        printf("\n");
    }
    printf("Nª Colisiones: %d\n", cont);
}
