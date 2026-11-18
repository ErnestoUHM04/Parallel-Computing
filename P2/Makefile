CC = gcc
CFLAGS = -Wall -Wextra

all: crea practica2

crea: crea.c
	$(CC) $(CFLAGS) crea.c -o crea

matriz.txt: crea
	./crea

practica2: practica2.c matriz.txt
	$(CC) $(CFLAGS) practica2.c -o practica2

clean:
	rm -f crea practica2 matriz.txt resultados.txt