// Programa para realizar el cálculo de la Serie de Fourier usando procesos
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<sys/types.h>
// Por ahora aguanta 2100 procesos en mi Macbook Pro M4 con 10 nucleos de CPU y 16 GB de RAM
#define N 2200 // Número de procesos
#define SALTO 0.1 //Salto entre cada X
#define COLUMNAS 65 // valores entre -PI y PI

float funcion_an_bn(float x, int n);

int main(){
    // El proceso padre crea/limpia el archivo donde se guardarán los resultados
    // Este tiene que guardar el resultado de todoso los procesos
    FILE *res_crear = fopen("resultados.txt", "w");
    if (res_crear == NULL) {
        perror("Error al crear resultados.txt");
        return 1;
    }
    fclose(res_crear);

    // A0 = (POWER(3.1416,4))/25 +1
    float A0 = (pow(M_PI,4)/25)+ 1;  // <--------- SE CAMBIA POR INTEGRANTE

    // Esta matriz guarda los resultados de manera compartida entre los procesos
    // Está creando un espacio de 100 filas x 64 columnas
    float (*resultados)[COLUMNAS] = mmap(
        NULL,
        sizeof(float) * N * COLUMNAS,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, // esta parte es para que la memoria sea compartida entre los processos
        -1,
        0
    );

    // El proceso padre crea un proceso hijo por cada n
    for (int i = 1; i <= N; i++) { // la iteración se hace desde i=1 hasta n (infinito en teoría)
        pid_t pid = fork();// Se crea un proceso

        if (pid < 0) {
            perror("Error al crear el proceso hijo");
            exit(1);
        }
        // Se crean en teoría N procesos, representando a N filas, entonces cada proceso se encarga de una fila de la matriz de resultados

        if (pid == 0) {
            int j = 0;
            // --- CÓDIGO DEL PROCESO HIJO ---
            // Se tiene que hacer el cálculo para cada x dentro de nuestro rango
            float x = -M_PI; // Donde empieza el rango

            while(x < M_PI+0.1 && j < COLUMNAS){
                resultados[i-1][j] = funcion_an_bn(x,i);//nuestra iteración sería nuestra n
                x+= SALTO; // subimos hasta topar con el rango mayor
                j++;
            }
            // El hijo termina su ejecución
            exit(0);
        }
    }

    // --- CÓDIGO DEL PROCESO PADRE ---

    // Para guardar la sumatoria se tiene que guardar un array de 64, para guardar el valor de la sumatoria de N para cada x
    float Fourier[COLUMNAS];

    // El padre espera a que terminen todos los procesos hijos
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Una vez que todos los hijos terminaron su parte podemos leer la matriz
    // ESTE PRINT ES PARA DEBUG
    // Lo que ahora se tiene que hacer es hacer la sumatoria de todos los resultados en N,
    // para luego guardar los valores para cada x
    /*
        for (int i = 0; i < N; i++) { // iterar por N
        for (int j = 0; j < COLUMNAS; j++) { // iterar por X
            //printf("resultados[%d][%d] = %f\n", i, j, resultados[i][j]); // DEBUG
        }
    }
    */
    float sum_n = 0;
    int x = 0;
    for (x = 0; x < COLUMNAS; x++){
        //Vamos iterar por x para llenar nuestro array de resultados
        for (int n = 0; n < N; n++){
            sum_n += resultados[n][x];
        }
        // una vez con el array con la sumatoria, falta agregar el valor de A0
        Fourier[x] = A0 + sum_n; // Guardamos el valor de la sumatoria
        sum_n = 0; // el valor de la suma se resetea a 0 después de trabajar con un x
        //printf("Fourier[%d]: %f \n", x, Fourier[x]); // x en este caso va de 0 a 64, no de -PI a PI, hay que convertir después
    }

    // Ahora que vemos que sí los tiene, vamos a meterlos al txt de resultados
    FILE *res_escribir = fopen("resultados.txt", "a");
    if (res_escribir != NULL) {
        for(x = 0; x < COLUMNAS-1; x++)
            fprintf(res_escribir,"%f\n", Fourier[x]);
        fclose(res_escribir);
    }

    return 0;
}

float funcion_an_bn(float x, int n){ // en teoría se deben de recibir valores de n >= 1
    //Aquí se carga la función resultante
    float AN =(2*((((pow(M_PI, 4)+5)*pow(n, 4)-12*pow(M_PI, 2)*pow(n, 2)+24)*sin(M_PI*n))+4*M_PI*n*(pow(M_PI, 2)*pow(n, 2)-6)*cos(M_PI*n))/(5*M_PI*pow(n, 5)));
    float BN = 0;
    return (AN * cos(n * x))+(BN * sin(n * x));
}