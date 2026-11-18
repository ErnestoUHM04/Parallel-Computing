// Programa básico de MPI visto en clase
#include<stdio.h>
#include<mpi.h>

int main(int argc,char **argv)
{
    int procesos;
    int identificador;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos); // esta llamada obtiene el numero de procesos que se estan ejecutando
    MPI_Comm_rank(MPI_COMM_WORLD, &identificador); // esta llamada obtiene el identificador del proceso que se esta ejecutando

    printf("Hola desde el proceso %d de %d\n", identificador, procesos);

    MPI_Finalize();
    return 0;
}