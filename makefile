#opciones de compilacion, muestra todos los warnings (-Wall)
CC=gcc -Wall

#carpeta de las cabeceras
HEADER_FILES_DIR = .
#opciones de compilacion, indica donde estan los archivos .h
INCLUDES = -I $(HEADER_FILES_DIR) -g

#nombre del ejecutable o archivo de salida
OUTPUT = ejecutable

#ficheros .h.
LIB_HEADERS = $(HEADER_FILES_DIR)/analizador_lexico.h $(HEADER_FILES_DIR)/analizador_sintactico.h $(HEADER_FILES_DIR)/definiciones.h $(HEADER_FILES_DIR)/errores.h $(HEADER_FILES_DIR)/lista.h $(HEADER_FILES_DIR)/sistema_de_entrada.h $(HEADER_FILES_DIR)/tabla_hash_encadenamiento.h $(HEADER_FILES_DIR)/TS.h

#FUENTES: todos los archivos .c necesarios
SRCS = main.c analizador_lexico.c analizador_sintactico.c errores.c sistema_de_entrada.c lista.c tabla_hash_encadenamiento.c TS.c

#ficheros .o: todos los .o con un analogo .c en SRCS
OBJS = $(SRCS:.c=.o)

#REGLA 1: genera el ejecutable, dependencia de los .o 
#si no hay librerias adicionales, no existe la variable $(LIBS),
#por lo que se elimina $(LIBS) de la regla siguiente
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS)
	
#REGLA 2: genera los .o cuando es necesario, dependencia de los .c y .h
#solo genera los .o necesarios por cambios en los .c o .h
#$@ es el nombre del fichero que se genera con la regla (.o)
#$< es el nombre del primer prerrequisito archivo .c cuyo .o se esta generando
%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)
	
# REGLA 3: borra el ejecutable (prerrequisito: clean)
cleanall: clean
	rm   -f $(OUTPUT) 

# REGLA 4: borra los ficheros .o y los de backup (terminan en ~)
clean:
	rm -f *.o *~
