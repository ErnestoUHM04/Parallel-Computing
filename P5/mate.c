// Programa básico de MPI para que cada NUCLEO haga una operación matemática distinta
// Yo tengo 10 NUCLEOS
#include<stdio.h>
#include<math.h>
#include<mpi.h>

#define NUCLEOS 10

int main(int argc, char **argv)
{
	int identificador,cuenta;
	MPI_Status estatus;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&identificador);

    float resultados[NUCLEOS];

	if (identificador==0)
	{
        resultados[identificador] = sqrt(16.0);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
	}
	else if (identificador==1)
	{
        resultados[identificador] = pow(2.0, 3.0);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
	}
    else if (identificador==2)
    {
        resultados[identificador] = log(2.71828);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==3)
    {
        resultados[identificador] = sin(3.14159 / 2);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==4)
    {
        resultados[identificador] = cos(0);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==5)
    {
        resultados[identificador] = tan(0);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==6)
    {
        resultados[identificador] = exp(1.0);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==7)
    {
        resultados[identificador] = log(0.1);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==8)
    {
        resultados[identificador] = floor(3.7);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }
    else if (identificador==9)
    {
        resultados[identificador] = ceil(3.3);
        printf("Nodo %d. Realizando operación matemática.\n", identificador);
        printf("El resultado de la operación matemática es: %.2f\n", resultados[identificador]);
    }

    MPI_Finalize();
}
