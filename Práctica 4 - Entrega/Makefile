CC=c99 
LDFLAGS=-lpthread
EJECUTABLES=primera_parte segunda_parte

# compila todo
all: $(EJECUTABLES)

# borra los ficheros objeto y los ejecutables
clean:
	rm -f *.o $(EJECUTABLES)

# reglas para generar cada ejecutable
# $(CC) es el compilador de C
# $@ es la parte izquierda de la regla (lo que queremos obtener)
# $^ son las dependencias de la regla (los ficheros que hay después de los ":")
# Ojo, en la línea que empieza por "$(CC)..." el primer carácter debe ser un TABULADOR
primera_parte: primera_parte.o semaforo.o recurso.o
	$(CC) -o $@ $^ $(LDFLAGS)

segunda_parte: segunda_parte.o semaforo.o
	$(CC) -o $@ $^ $(LDFLAGS)

# reglas para generar los objetos intermedios
recurso.o: recurso.h
semaforo.o: semaforo.h