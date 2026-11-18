#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Compilacion: gcc Servidor_Stream.c -o Servidor_UDP
// Ejecucion:   ./Servidor_UDP [Puerto]

int main(int argc, char **argv)
{
   // El puerto se recibe como argumento de la linea de comandos.
   if (argc != 2)
   {
      fprintf(stderr, "Uso: %s [Puerto]\n", argv[0]);
      return EXIT_FAILURE;
   }

   char *fin;
   long puerto = strtol(argv[1], &fin, 10);
   if (*argv[1] == '\0' || *fin != '\0' || puerto < 1 || puerto > 65535)
   {
      fprintf(stderr, "El puerto debe ser un numero entre 1 y 65535.\n");
      return EXIT_FAILURE;
   }

   // Direcciones IPv4 del servidor y del cliente que envie cada datagrama.
   struct sockaddr_in servidor = {0};
   struct sockaddr_in cliente = {0};
   socklen_t longitud_cliente;

   servidor.sin_family = AF_INET;              // Usaremos direcciones IPv4.
   servidor.sin_port = htons((unsigned short)puerto); // El puerto en orden de red.
   servidor.sin_addr.s_addr = htonl(INADDR_ANY); // Acepta datagramas por cualquier interfaz.

   // SOCK_DGRAM crea un socket UDP: no hay conexion que establecer.
   int fd = socket(AF_INET, SOCK_DGRAM, 0);
   if (fd < 0)
   {
      perror("Error al crear el socket");
      return EXIT_FAILURE;
   }

   // Asocia el socket con el puerto local para que pueda recibir datagramas.
   if (bind(fd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
   {
      perror("Error en bind");
      close(fd);
      return EXIT_FAILURE;
   }

   printf("Servidor UDP escuchando en el puerto %ld...\n", puerto);

   // UDP no usa listen() ni accept(): se reciben datagramas directamente.
   while (1)
   {
      char mensaje[1024];
      longitud_cliente = sizeof(cliente);

      // recvfrom espera un datagrama y guarda tambien la direccion del remitente.
      ssize_t bytes_recibidos = recvfrom(
         fd,
         mensaje,
         sizeof(mensaje) - 1, // Se reserva un byte para terminar el texto con '\0'.
         0,
         (struct sockaddr *)&cliente,
         &longitud_cliente
      );
      if (bytes_recibidos < 0)
      {
         perror("Error en recvfrom");
         close(fd);
         return EXIT_FAILURE;
      }

      mensaje[bytes_recibidos] = '\0';
      printf("Datagrama recibido: %s\n", mensaje);

      // sendto envia la respuesta al cliente que acaba de enviar el datagrama.
      const char respuesta[] = "Bienvenido a mi servidor.\n";
      if (sendto(
             fd,
             respuesta,
             sizeof(respuesta) - 1,
             0,
             (struct sockaddr *)&cliente,
             longitud_cliente
          ) < 0)
      {
         perror("Error en sendto");
         close(fd);
         return EXIT_FAILURE;
      }
   }

   close(fd);
   return EXIT_SUCCESS;
}
