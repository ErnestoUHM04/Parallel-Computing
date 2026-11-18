#include <mpi.h>
#include <stdio.h>

int main (int argc,char *argv[])
{
	int procesos,identificador;
	int longitud;
	char nombre[40];
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&procesos);
	MPI_Comm_rank(MPI_COMM_WORLD,&identificador);
	
	MPI_Get_processor_name(nombre,&longitud);
	printf("[Maquina %s]> Proceso %d de %d: Hola Mundo!\n",nombre,identificador,procesos);
	
	MPI_Finalize();
}


