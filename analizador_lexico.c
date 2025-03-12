
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistema_de_entrada.h"
#include "TS.h"
#include "analizador_lexico.h"
#include "definiciones.h"
#include "errores.h"

//Variable para enviar el operador ; al finalizar una línea en determinados contextos
int final_linea = 0;

//Función del autómata del patrón del identificador que se corresponde con las palabras reservadas
TIPOELEMENTO identificador(int c) {
    //Variable de retorno inicializada
    TIPOELEMENTO ret = {0};
    c=sig_caracter(); //Se lee el siguiente carácter de la cadena
    //En el momento en el que se encuentra un carácter que termina con el autómata, se termina el tratamiento de la cadena y se procede a recuperar el lexema
    while (c != EOF && c != '.' && c  != ' ' && c != '\n' && c != ')' && c != '(' && c != ',' && c != '{' && c != '}' && c != '[' && c != ']' && c!='+') {
        c=sig_caracter(); //Se lee el siguiente carácter de la cadena
    }
    //Se recupera el lexema desde el sistema de entrada y se almacena en el struct que se devuelve al analizador sintáctico
    ret.lexema = recuperar_lexema();
    //Se recupera de la tabla de símbolos el valor del identificador o la palabra reservada
    ret.complexico = buscar(ret.lexema);
    //Los identificadores son uno de los elementos que si están al final de la línea, se debe devolver un ; al finalizar la líne
    //Por esta razón se actualiza el valor de la variable global que permite enviar el ; al finalizar la línea
    if (ret.complexico == ID) {
        final_linea=1;
    }
    //Se retorna la pareja de componente léxico y lexema
    return ret;
}

//Función de los autómatas de los números en base 10
TIPOELEMENTO numero_base_diez(int c) {
    //Número simple o decimal o exponentes o imaginarios
    //Variables auxiliares
    TIPOELEMENTO ret = {0};
    int decimal = 0, exponente=0;
    int barra_baja = 0; //Variable para controlar las barras bajas en los números
    if (c == '.') { //Si el número solo tiene un número en la parte entera y después el . se detecta como número decimal
        decimal = 1;
    }
    //Se leen carácteres hasta encontrar algún delimitador que termine la aceptación de la cadena o haga que se rechace la cadena y se divida la cadena en ese punto si cumple con alguno de los patrones
    while (c != '+' && c != '-' && c != '/' && c != ' ' && c != '\n' && c != ',' && c != ')' && c != '(' && c != '{' && c != '}' && c != '[' && c != ']' && c != ':' && c != '*'){
        if (c == 'e' || c == 'E') { //Si detecta una e/E es que el número tiene un exponente
            if (exponente) { //Si se detecta otra e/E el autómata se termina en ese momento y se envía el lexema cortado en ese punto, en la próxima iteración del análisis se empezará en esa e/E
                emitir_error(3);
                break;
            }
            c=sig_caracter(); //Después de encontrar la e/E, existe la posibilidad de encontrar un + o - referenciando el tipo de exponente
            if (c == '+' || c == '-') {
                exponente=1;
                c=sig_caracter();
                continue;
            }
            exponente=1;
        }
        if (c == '.' ) { //Si aún no se ha encontrado un . que defina al número como decimal, se avanza marcando el número como decimal
            if (decimal) { //Si ya se ha encontrado un . con anterioridad, se termina el autómata y se recupera el lexema
                break;
            }
            decimal = 1;
        }
        if (c == '_') { //Tratamiento del carácter _, que no puede aparecer dos veces seguidas
            if (barra_baja) {
                emitir_error(4);
                break;
            }
            barra_baja = 1;
            continue;
        }
        barra_baja = 0;
        c=sig_caracter(); //Lectura del siguiente carácter
    }
    //Recuperación del lexema y devolución del componente léxico correspondiente, dependiendo de la prioridad de los tipos de números
    ret.lexema = recuperar_lexema();
    if (barra_baja) { //Si el último elemento de la cadena es un _, el número no es válido y se retrocede para formar el número con la parte válida y después se creará la parte de la _
        emitir_error(4);
        ret.lexema[strlen(ret.lexema)-1] = '\0';
        retroceder_delantero();
        retroceder_inicio();
    }
    if (ret.lexema[strlen(ret.lexema)-1] == 'i') { //Caso del número imaginario
        ret.complexico = IMAGINARIO;
        return ret;
    }
    if (exponente) { //Caso de exponente
        ret.complexico = FLOAT_EXPONENTE;
        return ret;
    }
    if (decimal) { //Caso de decimal
        ret.complexico = FLOAT_DECIMAL;
        return ret;
    }
    //Caso de los números enteros naturales
    ret.complexico = INT_NATURAL;
    return ret;
}

//Función del autómata de números que comienzan por un dígito
TIPOELEMENTO numero(int c) {
    //Variable de retorno inicializada
    TIPOELEMENTO ret = {0};
    if (c == '0') { //Los números binarios y hexadecimales tienen que empezar por un 0 para ser válidos y seguir por la letra B o H respectivamente
        int barra_baja = 0; //Variable para controlar las barras bajas en los números
        c=sig_caracter();
        switch (c) {
            case 'b': case 'B': //Binarios: el segundo carácter es b o B
                c=sig_caracter();
                while (c == '0' || c == '1' || c == '_') {
                    if (c == '_') { //Tratamiento del carácter _, que no puede aparecer dos veces seguidas
                        if (barra_baja) {
                            break;
                        }
                        barra_baja = 1;
                        continue;
                    }
                    barra_baja = 0;
                    c=sig_caracter();//Se leen los carácteres hasta encontrar un símbolo que termine la ejecución del autómata
                }
                //Al terminar la ejecución del autómata, se recupera el lexema al completo y se devuelve el componente léxico
                ret.lexema = recuperar_lexema();
                if (barra_baja) {//Si el último elemento de la cadena es un _, el número no es válido y se retrocede para formar el número con la parte válida y después se creará la parte de la _
                    emitir_error(4);
                    ret.lexema[strlen(ret.lexema)-1] = '\0';
                    retroceder_delantero();
                    retroceder_inicio();
                }
                if (ret.lexema[strlen(ret.lexema)-1] == 'i') { //Caso del número imaginario, ya que tiene prioridad
                    ret.complexico = IMAGINARIO;
                    return ret;
                }
                ret.complexico = INT_BINARIO;
                return ret;
            case 'x': case 'X': //Hexadecimales: el segundo carácter es x o X
                c=sig_caracter();
                while (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
                    if (c == '_') { //Tratamiento del carácter _, que no puede aparecer dos veces seguidas
                        if (barra_baja) {
                            break;
                        }
                        barra_baja = 1;
                        continue;
                    }
                    barra_baja = 0;
                    c=sig_caracter();//Se leen los carácteres hasta encontrar un símbolo que termine la ejecución del autómata
                }
                //Al terminar la ejecución del autómata, se recupera el lexema al completo y se devuelve el componente léxico
                ret.lexema = recuperar_lexema();
                if (barra_baja) {//Si el último elemento de la cadena es un _, el número no es válido y se retrocede para formar el número con la parte válida y después se creará la parte de la _
                    emitir_error(4);
                    ret.lexema[strlen(ret.lexema)-1] = '\0';
                    retroceder_delantero();
                    retroceder_inicio();
                }
                if (ret.lexema[strlen(ret.lexema)-1] == 'i') {//Caso del número imaginario, ya que tiene prioridad
                    ret.complexico = IMAGINARIO;
                    return ret;
                }
                ret.complexico = INT_HEXADECIMAL;
                return ret;
            default: //Caso estándar de un número en base 10 que empieza por 0
                return numero_base_diez(c);
        }
    }
    //Caso estándar de un número en base 10 que empieza por 0
    return numero_base_diez(c);
}

//Función del autómata de los números que empiezan por . o para el operador de .
TIPOELEMENTO numero_decimal(int c) {
    //Variable de retorno inicializada
    TIPOELEMENTO ret = {0};
    c=sig_caracter(); //Se lee el siguiente carácter de la cadena
    if (isdigit(c)) {
        //Variables auxiliares
        int exponente=0;
        //Se leen carácteres hasta encontrar algún delimitador que termine la aceptación de la cadena o haga que se rechace la cadena y se divida la cadena en ese punto si cumple con alguno de los patrones
        while (c != '+' && c != '-' && c != '/' && c != ' ' && c != '\n' && c != ',' && c != ')' && c != '(' && c != '{' && c != '}' && c != '[' && c != ']' && c != ':' && c != '*'){
            if (c == 'e' || c == 'E') { //Si detecta una e/E es que el número tiene un exponente
                if (exponente) { //Si se detecta otra e/E el autómata se termina en ese momento y se envía el lexema cortado en ese punto, en la próxima iteración del análisis se empezará en esa e/E
                    emitir_error(3);
                    break;
                }
                c=sig_caracter(); //Después de encontrar la e/E, existe la posibilidad de encontrar un + o - referenciando el tipo de exponente
                if (c == '+' || c == '-') {
                    exponente=1;
                    c=sig_caracter();
                    continue;
                }
                exponente=1;
            }
            c=sig_caracter(); //Lectura del siguiente carácter
            if (c == '.' ) { //Si aún no se ha encontrado un . que defina al número como decimal, se avanza marcando el número como decimal
                break;
            }
        }
        //Recuperación del lexema y devolución del componente léxico correspondiente
        ret.lexema = recuperar_lexema();
        //Se añade el retroceso del final de los autómatas de números para tratar el delimitador que se ha encontrado
        retroceder_delantero();
        retroceder_inicio();
        if (ret.lexema[strlen(ret.lexema)-1] == 'i') { //Caso del número imaginario
            ret.complexico = IMAGINARIO;
            return ret;
        }
        if (exponente) { //Caso de exponente
            ret.complexico = FLOAT_EXPONENTE;
            return ret;
        }
        //Caso restante de un número decimal, ya que empieza por .
        ret.complexico = FLOAT_DECIMAL;
        return ret;
    }
    //Operador del punto "."
    //Se retrocede el puntero delantero del sistema de entrada para volver a tener el carácter del punto
    retroceder_delantero();
    retroceder_delantero();
    c=sig_caracter();
    //Se actualiza inicio para equilibrar el sistema de entrada, porque al no ejecutar recuperar_lexema(), se necesita poner a la par ambos punteros
    actualizar_inicio();
    //Se realiza la reserva de memoria al ser un tamaño determinado conocido. Es un solo carácter y se sabe por cumplir el autómata del operador en solitario
    ret.lexema = malloc(2);
    ret.lexema[0] = (char) c; //Se añade el carácter del . y se añade el final de cadena porque el tipo de dato de la estructura es char* (string)
    ret.lexema[1] = '\0';
    ret.complexico=c; //Se devuelve como componente léxico el ASCI del carácter
    return ret;
}

//Función para los autómatas de los operadores
TIPOELEMENTO operador(int c) {
    //Variable de retorno inicializada
    TIPOELEMENTO ret = {0};
    switch(c) { //Se analiza el operador que se ha leído para escoger el camino/autómata que se debe ejecutar
        case '=':
            c=sig_caracter();
            if (c == '=') { //Caso del operador ==
                actualizar_inicio(); //Se equilibran los punteros al usar una forma básica de generación del lexema, al saber exactamente el lexema que hay que construir
                ret.lexema = malloc(3);
                ret.lexema[0] = '=';
                ret.lexema[1] = '=';
                ret.lexema[2] = '\0'; //Se añade el fin de cadena
                ret.complexico = OPER_IGUAL_COMPARACION;
                final_linea = 0; //La mayoría de operadores no provocan que se tenga que devolver un ; al finalizar la línea
                return ret;
            }
            //En el caso de que el operador no esté continuado por otro =, se devuelve el carácter =, por eso se vuelve a leer el carácter y se construirá el struct a devolver al salir del switch
            retroceder_delantero();
            retroceder_delantero();
            c=sig_caracter();
            break;
        case ':':
            c=sig_caracter();
            if (c == '='){ //Caso del operador :=, similar que el caso anterior
                actualizar_inicio();
                ret.lexema = malloc(3);
                ret.lexema[0] = ':';
                ret.lexema[1] = '=';
                ret.lexema[2] = '\0';
                ret.complexico = OPER_DECLARACION;
                final_linea = 0; //La mayoría de operadores no provocan que se tenga que devolver un ; al finalizar la línea
                return ret;
            }
            //Se vuelve al carácter inicial para recuperarlo en solitario, el :
            retroceder_delantero();
            retroceder_delantero();
            c=sig_caracter();
            break;
        case '+':
            c=sig_caracter();
            if (c == '=') { //Caso del operador +=, similar a los casos anteriores
                actualizar_inicio();
                ret.lexema = malloc(3);
                ret.lexema[0] = '+';
                ret.lexema[1] = '=';
                ret.lexema[2] = '\0';
                ret.complexico = OPER_ADICION;
                final_linea = 0; //La mayoría de operadores no provocan que se tenga que devolver un ; al finalizar la línea
                return ret;
            }
            //Se vuelve al carácter inicial para recuperarlo en solitario, el +
            retroceder_delantero();
            retroceder_delantero();
            c=sig_caracter();
            break;
        case '<':
            c=sig_caracter();
            if (c == '-') { //Caso del operador <-, similar a los casos anteriores
                actualizar_inicio();
                ret.lexema = malloc(3);
                ret.lexema[0] = '<';
                ret.lexema[1] = '-';
                ret.lexema[2] = '\0';
                ret.complexico = OPER_DIRECCION;
                final_linea = 0; //La mayoría de operadores no provocan que se tenga que devolver un ; al finalizar la línea
                return ret;
            }
            //Se vuelve al carácter inicial para recuperarlo en solitario, el <
            retroceder_delantero();
            retroceder_delantero();
            c=sig_caracter();
            break;
        case '/':
            c=sig_caracter();
            if (c == '/') { //Caso del comentario de una línea
                while (c != '\n') { //Mientras no se termina la línea se sigue considerando un comentario, por lo tanto, se va leyendo y se va moviendo el puntero delantero porque no es necesario mantener el inicio de un lexema que no se debe devolver su componente léxico
                    c=sig_caracter(); //Se consumen los carácteres del comentario
                    if (c == EOF) { //Si no se detecta el salto de línea y se termina el archivo se emite un error y se envía el fin de fichero para terminar el análisis
                        emitir_error(1);
                        ret.complexico = EOF;
                        return ret;
                    }
                    actualizar_inicio();
                }
                if (final_linea==1) { //Al terminar la línea, si el último elemento antes del comentario precisa que se devuelva el operador de ;, se devuelve el operador
                    ret.lexema = malloc(2);
                    ret.lexema[0] = ';';
                    ret.lexema[1] = '\0';
                    ret.complexico = ';';
                    final_linea=0; //La mayoría de operadores no provocan que se tenga que devolver un ; al finalizar la línea
                    return ret;
                }
                return sig_complex(); //Después de terminar el comentario se devuelve el siguiente componente léxico porque se tiene que devolver algo al analizador sintáctico
            }
            if (c == '*') { //Caso de comentario con cierre explícito
                actualizar_inicio();
                while (c != '/') { //Se debe cumplir el patrón de cierre
                    while (c != '*') {
                        c=sig_caracter(); //Se consumen los carácteres del comentario
                        actualizar_inicio(); //Se equilibra el puntero de inicio porque el carácter leído no se utiliza en la construcción del componente léxico
                    }
                    c=sig_caracter();
                    actualizar_inicio();
                }
                actualizar_inicio();
                return sig_complex();
            }
            //Se vuelve al carácter inicial para recuperarlo en solitario, la /
            retroceder_delantero();
            retroceder_delantero();
            c=sig_caracter();
            break;
        case '"': //Caso de los strings interpretados
            c=sig_caracter();
            while (c != '"' && c != '\n') {
                c=sig_caracter();
                if (c == '\\') { //Comodines/Wildcards
                    sig_caracter();
                    sig_caracter();
                }
            }
            if (c == '\n') { //Los strings interpretados no contemplan el salto de línea
                emitir_error(5);
                if (final_linea==1) { //Al terminar la línea, si el último elemento antes del comentario precisa que se devuelva el operador de ;, se devuelve el operador
                    ret.lexema = malloc(2);
                    ret.lexema[0] = ';';
                    ret.lexema[1] = '\0';
                    ret.complexico = ';';
                    final_linea=0;
                    return ret;
                }
            }
            sig_caracter(); //Para incluir el símbolo de "
            ret.lexema = recuperar_lexema();
            ret.complexico = STRING_LITERAL_INTERPRETADO;
            final_linea = 1;
            //Se retrocede para tratar el siguiente carácter
            retroceder_delantero();
            retroceder_inicio();
            return ret;
        case '`': //Caso de los strings raw/crudos
            c=sig_caracter();
            while (c != '`') {
                c=sig_caracter();
                if (c == '\\') {
                    sig_caracter();
                    sig_caracter();
                }
            }
            sig_caracter(); //Para incluir el símbolo de "
            ret.lexema = recuperar_lexema();
            ret.complexico = STRING_LITERAL_RAW;
            final_linea = 1;
            //Se retrocede para tratar el siguiente carácter
            retroceder_delantero();
            retroceder_inicio();
            return ret;
        default:
            break;
    }
    actualizar_inicio();
    //Se prepara el elemento a entregar al analizador sintáctico siendo un solo carácter marcando el final del string
    ret.lexema = malloc(2);
    ret.lexema[0] = (char) c;
    ret.lexema[1] = '\0';
    ret.complexico = c;
    if (c == ')' || c == ']' || c == '}') { //Los delimitadores que provocan que se tenga que devolver ; al finalizar la línea
        final_linea=1;
    } else {
        final_linea=0;
    }
    return ret;
}

//Función principal del analizador léxico, devuelve la pareja lexema y componente léxico
TIPOELEMENTO sig_complex() {
    //Inicialización de variables
    TIPOELEMENTO sig = {0};
    int c=0;
    //Lectura del primer carácter
    c=sig_caracter();
    while (c == ' ' || c == '\n') { //Se consumen los espacios y saltos de línea
        actualizar_inicio();
        if (c == '\n' && final_linea==1) { //Los saltos de línea marcan el momento de entregar el operador ; si se cumplen las condiciones representadas en las funciones de los autómatas
            sig.lexema = malloc(2);
            sig.lexema[0] = ';';
            sig.lexema[1] = '\0';
            sig.complexico = ';';
            final_linea=0;
            return sig;
        }
        c=sig_caracter();
    }
    if (isalpha(c) || c == '_') { //Caso del patrón de los identificadores y palabras reservadas
        sig = identificador(c);
        //Se retrocede para tratar el carácter delimitador
        retroceder_delantero();
        retroceder_inicio();
        return sig;
    }
    if (isdigit(c)) { //Caso en el que el primer carácter de la cadena sea un dígito
        sig = numero(c);
        //Se retrocede para tratar el carácter delimitador
        retroceder_delantero();
        retroceder_inicio();
        final_linea=1;
        return sig;
    }
    if (c == '.') { //Caso en el que el primer carácter de la cadena sea un dígito
        sig = numero_decimal(c);
        final_linea=1;
        return sig;
    }
    sig = operador(c); //Resto de casos
    return sig;
}

//Función de terminación
void cerrar_analizador() {
    cerrar_entrada();
}