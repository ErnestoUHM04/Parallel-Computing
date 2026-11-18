// Programa para realizar el cálculo de la Serie de Fourier usando procesos e hilos
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define N 100                   // Número de procesos (uno por cada n)
#define SALTO 0.1               // Salto entre cada X
#define COLUMNAS 65             // Valores entre -PI y PI
#define HILOS_POR_PROCESO 100   // Hilos que creará cada proceso hijo

float funcion(float x, int n, int caso) {
    switch(caso) {
    case 1: {
        float AN = (2 * ((((pow(M_PI, 4) + 5) * pow(n, 4) - 12 * pow(M_PI, 2) * pow(n, 2) + 24) * sin(M_PI * n)) +
                         4 * M_PI * n * (pow(M_PI, 2) * pow(n, 2) - 6) * cos(M_PI * n)) /
                        (5 * M_PI * pow(n, 5)));
        float BN = 0;
        return (AN * cos(n * x)) + (BN * sin(n * x));
    }
    case 2:
        return (-1.0f / 15.0f) * (1.0f / pow(n, 3) *
               (((5.0f * pow(M_PI * n, 2)) - (6.0f * pow(n, 2)) - 30.0f) * pow(-1, n)) * sin(n * x));
    case 3:
        return -(4.0f / 3.0f) * ((pow(M_PI * n, 2) - 6.0f) * pow(-1, n)) * (1 / pow(n, 4)) * cos(n * x);
    case 4:
        return (2.0f * pow(-1, n) * ((1.0f - pow(M_PI, 2)) * pow(n, 2) + 6.0f) * sin(n * x)) / pow(n, 3);
    default:
        return -1.0f;
    }
}

// Estructura para pasar argumentos a cada hilo
typedef struct {
    int n;
    int caso;
    float (*resultados)[COLUMNAS];
    int thread_id;
    int num_threads;
} thread_arg_t;

// Función que ejecuta cada hilo: calcula las columnas que le tocan
void* hilo_calcular(void* arg) {
    thread_arg_t* a = (thread_arg_t*)arg;
    int fila = a->n - 1;  // Fila en la matriz de resultados (n-1)

    for (int j = a->thread_id; j < COLUMNAS; j += a->num_threads) {
        float x = -M_PI + j * SALTO;
        a->resultados[fila][j] = funcion(x, a->n, a->caso);
    }
    return NULL;
}

void calcular_fourier(float A0, char nombre[], int caso) {
    char archivo[50];
    snprintf(archivo, sizeof(archivo), "resultados_%s.txt", nombre);

    // Memoria compartida entre procesos
    size_t shm_size = sizeof(float) * N * COLUMNAS;
    float (*resultados)[COLUMNAS] = mmap(
        NULL, shm_size, PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0
    );
    if (resultados == MAP_FAILED) {
        perror("Error en mmap");
        exit(1);
    }

    // Crear N procesos hijos
    for (int i = 1; i <= N; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error al crear el proceso hijo");
            exit(1);
        }

        if (pid == 0) {
            // --- CÓDIGO DEL HIJO ---
            pthread_t hilos[COLUMNAS];
            thread_arg_t args[COLUMNAS];

            // Crear 100 hilos
            for (int t = 0; t < COLUMNAS; t++) {
                args[t].n = i;
                args[t].caso = caso;
                args[t].resultados = resultados;
                args[t].thread_id = t;
                args[t].num_threads = COLUMNAS;

                if (pthread_create(&hilos[t], NULL, hilo_calcular, &args[t]) != 0) {
                    perror("Error al crear hilo");
                    exit(1);
                }
            }

            // Esperar a que terminen todos los hilos
            for (int t = 0; t < COLUMNAS; t++) {
                pthread_join(hilos[t], NULL);
            }

            exit(0);
        }
    }

    // --- CÓDIGO DEL PADRE ---
    // Esperar a que terminen todos los procesos hijos
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Sumar los resultados de todas las filas para cada x
    float Fourier[COLUMNAS];
    float sum_n = 0;
    for (int x = 0; x < COLUMNAS; x++) {
        for (int n = 0; n < N; n++)
            sum_n += resultados[n][x];
        Fourier[x] = A0 + sum_n;
        sum_n = 0;
    }

    // Guardar resultados en archivo
    FILE *res_escribir = fopen(archivo, "w");
    if (res_escribir != NULL) {
        for (int x = 0; x < COLUMNAS - 1; x++) {
            fprintf(res_escribir, "%f\n", Fourier[x]);
        }
        fclose(res_escribir);
    }

    munmap(resultados, shm_size);
}

int main() {
    char nombres[4][50] = {"ernesto", "uriel", "omar", "diego"};

    float A0_ernesto = (pow(M_PI, 4) / 25) + 1;
    float A0_uriel = 0.0f;
    float A0_omar = -(pow(M_PI, 4) - 150.0f) * (1.0f / 30.0f);
    float A0_diego = 3;
    float A0[4] = {A0_ernesto, A0_uriel, A0_omar, A0_diego};

    for (int i = 0; i < 4; i++) {
        calcular_fourier(A0[i], nombres[i], i + 1);
    }

    // Crear CSV con todos los resultados
    FILE *csv = fopen("resultados_fourier.csv", "w");
    if (csv == NULL) {
        perror("Error al crear el CSV");
        return 1;
    }

    fprintf(csv, "x,ernesto,uriel,omar,diego\n");

    FILE *archivos[4];
    for (int i = 0; i < 4; i++) {
        char nombre_archivo[50];
        snprintf(nombre_archivo, sizeof(nombre_archivo), "resultados_%s.txt", nombres[i]);
        archivos[i] = fopen(nombre_archivo, "r");
        if (archivos[i] == NULL) {
            perror("Error al abrir archivo de resultados");
            fclose(csv);
            return 1;
        }
    }

    for (int i = 0; i < COLUMNAS - 1; i++) {
        float resultados[4];
        for (int j = 0; j < 4; j++) {
            fscanf(archivos[j], "%f", &resultados[j]);
        }
        float x = -M_PI + (i * SALTO);
        fprintf(csv, "%f,%f,%f,%f,%f\n", x, resultados[0], resultados[1], resultados[2], resultados[3]);
    }

    fclose(csv);
    for (int i = 0; i < 4; i++) {
        fclose(archivos[i]);
    }

    return 0;
}