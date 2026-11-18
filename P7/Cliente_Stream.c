#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
//$gcc Cliente_Stream.c -o Cliente_S
//$./Cliente_S [Dirección IP del servidor] [Puerto del servidor]

int main(int argc, char *argv[])
{
   if(argc > 2)
   {
      char *ip;
	  int fd,numbytes,puerto;
	  char buf[100];
	  puerto=atoi(argv[2]);
	  ip=argv[1];
	  struct hostent *he;
	  struct sockaddr_in servidor;
	  if ((he=gethostbyname(ip))==NULL)
	  {
	     printf("gethostbyname() error\n");
		 exit(-1);
	  }
      if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	  {
	     printf("socket() error\n");
		 exit(-1);
      }
	  servidor.sin_family = AF_INET;
	  servidor.sin_port = htons(puerto);
	  servidor.sin_addr = *((struct in_addr *)he->h_addr);
	  bzero(&(servidor.sin_zero),8);
	  if(connect(fd, (struct sockaddr *)&servidor,sizeof(struct sockaddr))==-1)
	  {
	     printf("connect() error\n");
		 exit(-1);
	  }
	  if ((numbytes=recv(fd,buf,100,0)) == -1)
	  {
	     printf("Error en recv() \n");
		 exit(-1);
      }
	  buf[numbytes]='\0';
	  printf("Mensaje del Servidor: %s\n",buf);
	  close(fd);
   }
   else
   {
      printf("No se ingreso dirección ip ni puerto en los parametros\n");
   }
}
