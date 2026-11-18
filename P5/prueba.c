// Programa de ejemplo para probar MPI con información adicional del procesador
#include<stdio.h>
#include<mpi.h>


int main(int argc, char *argv[]) {

    int rank;
    int size;
    char nombre_procesador[MPI_MAX_PROCESSOR_NAME];
    int longitud;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Get_processor_name(nombre_procesador, &longitud);

    printf("Hola desde el proceso %d de %d | Procesador: %s\n",
           rank, size, nombre_procesador);

    MPI_Finalize();

    return 0;
}