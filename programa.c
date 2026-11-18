#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
  pid_t pid;
  // PID significa Process Identification Number.
  // Es un número entero que identifica de manera única a un proceso en ejecución en el sistema operativo.

  pid = fork(); // fork es una llamada al sistema.
  // NO ES UNA FUNCIÓN.   <--- ten cuidado que nos reprueba el profeta

  printf("%d", pid);
  if(pid == -1){
    perror("Error al crear el proceso. \n\n");
    exit(0);
  }
  if(pid == 0){
    printf("\n\nSoy el proceso hijo. \n\n");
    sleep(5);
    exit(0);
  }
  else{
    printf("\n\nSoy el proceso padre\n\n");
    sleep(5);
    exit(0);
  }
  /*
   En estas series de condicionales, se está ejecutando ambas, el hijo y el padre al mismo tiempo, al crear un proceso hijo que ejecute solamente una parte del código
   Esto no es paralelismo, ya que el proceso padre y el proceso hijo se ejecutan de manera secuencial, pero sí es concurrencia, ya que ambos procesos están en ejecución al mismo tiempo.
   */
  return 0;
}
