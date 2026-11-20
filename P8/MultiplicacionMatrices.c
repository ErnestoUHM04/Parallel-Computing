#include<stdio.h>
#define Filas 5
#define Columnas 5

int main()
{
	int A[Filas][Columnas]={{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
	int B[Filas][Columnas]={{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
	int C[Filas][Columnas]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
	int i,j,k	;
	
	printf("\nMatriz A");
	for(i=0;i<Filas;i++)
	{
		printf("\n");
		for(j=0;j<Columnas;j++)
		{
			printf("%d ",A[i][j]);
		}
	}
	printf("\nMatriz B");
	for(i=0;i<Filas;i++)
	{
		printf("\n");
		for(j=0;j<Columnas;j++)
		{
			printf("%d ",B[i][j]);
		}
	}
	printf("\nMatriz C antes de la multiplicacion");
	for(i=0;i<Filas;i++)
	{
		printf("\n");
		for(j=0;j<Columnas;j++)
		{
			printf("%d ",C[i][j]);
		}
	}

	for(i=0;i<Filas;i++)
	{
    	for(k=0;k<Columnas;k++)
    	{
        	for(j=0;j<Filas;j++)
            {
            	C[i][k]=C[i][k]+A[i][j]*B[j][k];
        	}
        }
	}

	printf("\nMatriz C despues de la multiplicacion");
	for(i=0;i<Filas;i++)
	{
		printf("\n");
		for(j=0;j<Columnas;j++)
		{
			printf("%d ",C[i][j]);
		}
	}
	return 0;
}
