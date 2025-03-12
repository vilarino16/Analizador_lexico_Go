
#ifndef SISTEMA_DE_ENTRADA_H
#define SISTEMA_DE_ENTRADA_H

//Función para abrir el archivo que se pasa por parámetros y cargar el primer bloque lógico
void inicializar_Sistema_Entrada(int n_param, char **parametros);

//Lectura del siguiente carácter del fichero
int sig_caracter();

//Función para recuperar el lexema analizado por el analizador léxico
char* recuperar_lexema();

//Función para retroceder el puntero inicio
void retroceder_inicio();

//Función para retroceder el puntero delantero
void retroceder_delantero();

//Función para cerrar el archivo de entrada
void cerrar_entrada();

//Función para actualizar inicio
void actualizar_inicio();

#endif //SISTEMA_DE_ENTRADA_H
