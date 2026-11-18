CC = gcc
CFLAGS = -Wall -Wextra

all: Cliente_Datagrama Servidor_Datagrama

Cliente_Datagrama: Cliente_Datagrama.c
	$(CC) $(CFLAGS) Cliente_Datagrama.c -o Cliente_Datagrama

Servidor_Datagrama: Servidor_Datagrama.c
	$(CC) $(CFLAGS) Servidor_Datagrama.c -o Servidor_Datagrama

clean:
	rm -f Cliente_Datagrama Servidor_Datagrama