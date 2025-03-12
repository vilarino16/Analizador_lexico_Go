cmake_minimum_required(VERSION 3.30)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c
        definiciones.h
        TS.h
        analizador_lexico.h
        analizador_sintactico.h
        TS.c
        tabla_hash_encadenamiento.c
        lista.c
        analizador_sintactico.c
        analizador_lexico.c
        errores.c
        errores.h
        sistema_de_entrada.c
        sistema_de_entrada.h)
