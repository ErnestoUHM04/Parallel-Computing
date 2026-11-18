#include <stdio.h>
#include <mpi.h>
 
int main(int argc, char *argv[]) 
{ 
   int identificador,procesos,mensaje;
   MPI_Status estatus;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &procesos);
   MPI_Comm_rank(MPI_COMM_WORLD, &identificador);
 
   if(identificador == 0)
   {
      MPI_Send(&identificador,1,MPI_INT,identificador+1,0,MPI_COMM_WORLD);
   }
   else
   {
      MPI_Recv(&mensaje,1,MPI_INT,identificador-1,0,MPI_COMM_WORLD,&estatus);
      printf("\nSoy el proceso %d y he recibido %d\n\n",identificador,mensaje);
      mensaje++;
      if(identificador!=procesos-1)
      {
         MPI_Send(&mensaje,1,MPI_INT,identificador+1,0,MPI_COMM_WORLD);
      }
   }
   MPI_Finalize();
} 


