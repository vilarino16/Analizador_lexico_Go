#!/bin/bash
if ! test $# -eq 3 #Comprobaciones de obtener el número de parámetros necesarios
then
    echo
    echo "Parámetros insuficentes. Ejemplo script.sh -f ruta/al/archivo/de/concurrentSum.go 0/1"
    echo "0(ejecucion simple)/1(ejecucion con valgrind)"
    exit 0
fi
if (! test -f $2 && ! test -r $2) #Comprobación de que el archivo tiene permisos de lectura
then 
    echo "Parámetro de tipo incorrecto. Asegúrese de que tiene permisos de lectura. Ejemplo script.sh -f ruta/al/archivo/de/concurrentSum.go"
    exit 0
fi
make
if test $3 -eq 0
then
  ./ejecutable -f concurrentSum.go
fi
if test $3 -eq 1
then
  valgrind --leak-check=full --track-origins=yes -s ./ejecutable -f concurrentSum.go
fi
exit 0
