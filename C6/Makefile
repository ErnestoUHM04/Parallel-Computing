CC = gcc
CFLAGS = -Wall -Wextra

all: Cliente_Fourier Servidor_Fourier

Cliente_Fourier: Cliente_Datagrama_Fourier_Nuevo.c
	$(CC) $(CFLAGS) Cliente_Datagrama_Fourier_Nuevo.c -o Cliente_Fourier

Servidor_Fourier: Servidor_Datagrama_Fourier_Nuevo.c
	$(CC) $(CFLAGS) Servidor_Datagrama_Fourier_Nuevo.c -o Servidor_Fourier

clean:
	rm -f Cliente_Fourier Servidor_Fourier