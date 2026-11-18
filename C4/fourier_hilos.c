// Programa para realizar el cálculo de la Serie de Fourier usando hilos
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>

#define N 1000 // Número de procesos
#define SALTO 0.1 //Salto entre cada X
#define COLUMNAS 65 // valores entre -PI y PI

// Se define una estructura llamada DatosHilo que contiene información relevante para cada hilo.
typedef struct{
   int numero_hilo;
   int *resultado;
   int (*matriz)[NUM_COLUMNAS]; // matriz es un apuntador a una matriz de enteros con NUM_COLUMNAS columnas. Esto permite que cada hilo tenga acceso a la matriz completa y pueda operar sobre ella según sea necesario.
} DatosHilo;

// Función que calcula el valor de la función de Fourier para un valor dado de x, n y caso.
float funcion(float x, int n, int caso){ // en teoría se deben de recibir valores de n >= 1
    switch(caso){
    case 1: {
        //Aquí se carga la función resultante
        // los calculos para la serie de ernesto
        float AN =(2*((((pow(M_PI, 4)+5)*pow(n, 4)-12*pow(M_PI, 2)*pow(n, 2)+24)*sin(M_PI*n))+4*M_PI*n*(pow(M_PI, 2)*pow(n, 2)-6)*cos(M_PI*n))/(5*M_PI*pow(n, 5)));
        float BN = 0;
        return (AN * cos(n * x))+(BN * sin(n * x));
        break;
    }
    case 2:
        //los calculos para la serie de uriel
        return  (-1.0f/15.0f) * (1.0f/pow(n,3) * (((5.0f * pow(M_PI*n,2)) - (6.0f * pow(n,2)) - 30.0f  )*pow(-1,n)) * sin(n*x));  
        break;
    case 3:
        //los calculos para la serie de omar
        return  -(4.0f/3.0f)*( (pow(M_PI*n,2) - 6.0f)*pow(-1,n))*(1/pow(n,4))*cos(n*x);
        break;
    case 4:
        // los calculos para la serie de diego
        return (2.0f * pow(-1, n) * ((1.0f - pow(M_PI, 2)) * pow(n, 2) + 6.0f) * sin(n * x)) / pow(n, 3);
        break;
    default:
        return 0-1.0f;
        break;
    }
}

void calcular_fourier(float A0, char nombre[], int caso){
    // El proceso padre crea/limpia el archivo donde se guardarán los resultados
    // Este tiene que guardar el resultado de todoso los procesos
    char archivo[50]= "resultados";
    snprintf(archivo, sizeof(archivo), "resultados_%s.txt", nombre);
    // Esta matriz guarda los resultados de manera compartida entre los procesos
    // Está creando un espacio de 100 filas x 64 columnas

    size_t shm_size = sizeof(float) * N * COLUMNAS;
    float (*resultados)[COLUMNAS] = mmap(
        NULL,
        shm_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );


    // El proceso padre crea un proceso hijo por cada n
    for (int i = 1; i <= N; i++) { // la iteración se hace desde i=1 hasta n (infinito en teoría)
        pid_t pid = fork();// <-------------------------------------- Se crea un proceso

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
                resultados[i-1][j] = funcion(x,i,caso);//nuestra iteración sería nuestra n
                //resultados[i-1][j] = funcion_ernesto(x,i);//nuestra iteración sería nuestra n
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
    FILE *res_escribir = fopen(archivo, "w");
    if (res_escribir != NULL) {
        for(x = 0; x < COLUMNAS-1; x++)
            fprintf(res_escribir,"%f\n", Fourier[x]);
        fclose(res_escribir);
    }

    munmap(resultados, shm_size);
}

void *Multiplicar_fila(void *argumentos) {
   DatosHilo *datos = (DatosHilo *)argumentos; // En esta línea, se realiza un casting del puntero de tipo void* a un puntero de tipo DatosHilo*.
   //Esto permite acceder a los miembros de la estructura DatosHilo a través del puntero datos.
   int numero_hilo = datos->numero_hilo; // Se accede al miembro numero_hilo de la estructura DatosHilo a través del puntero datos.
   int *resultado = datos->resultado; // Se accede al miembro resultado de la estructura DatosHilo a través del puntero datos.
   int (*matriz)[NUM_COLUMNAS] = datos->matriz; // Se accede al miembro matriz de la estructura DatosHilo a través del puntero datos.

   *(resultado) = 1; // Se inicializa el valor apuntado por resultado a 1. Esto es necesario para realizar la multiplicación correctamente, ya que cualquier número multiplicado por 1 permanece igual.

   for(int i = 0; i < NUM_COLUMNAS; i++)
      *(resultado) *= matriz[numero_hilo - 1][i]; // Se realiza la multiplicación de los elementos de la fila correspondiente de la matriz.

   pthread_exit(NULL); // Se finaliza la ejecución del hilo y se devuelve NULL como valor de retorno. Esto indica que el hilo ha terminado su trabajo y no tiene un valor específico para devolver.
}

int main (){
    // estos serán los casos de prueba, cada uno con su A0 correspondiente
    char nombres[4][50] ={ //miembros del equipo, cada uno con su serie
        "ernesto",
        "uriel",
        "omar",
        "diego"
    };

    float A0_ernesto = (pow(M_PI,4)/25)+ 1;  // <--------- SE CAMBIA POR INTEGRANTE
    float A0_uriel = 0.0f;
    float A0_omar = -(pow(M_PI,4) - 150.0f)*(1.0f/30.0f);
    float A0_diego= 3;
    float A0[4] = {A0_ernesto, A0_uriel, A0_omar, A0_diego };
    for(int i = 0; i < 4; i++){
        calcular_fourier(A0[i],nombres[i],i+1);
    }

    // Inicializamos los resultados de cada hilo
    int resultado_hilo[N] = {0}; // Se inicializa un arreglo de enteros llamado resultado_hilo con tamaño N y se establece cada elemento en 0. Esto se hace para almacenar los resultados de la multiplicación de cada hilo.

    pthread_t id_hilo[N]; // Se declara un arreglo de identificadores de hilos llamado id_hilo con tamaño N. Cada elemento del arreglo representará un hilo creado en el programa.
    DatosHilo datos_hilo[N];


    printf("Hola, soy el hilo principal y voy a crear 3 hilos...\n");
    for(int i = 0; i < NUM_FILAS; i++){
        datos_hilo[i].numero_hilo = i + 1;
        datos_hilo[i].resultado = &resultado_hilo[i];
        datos_hilo[i].matriz = matriz;
        pthread_create(&id_hilo[i], NULL, Multiplicar_fila, &datos_hilo[i]);
        pthread_join(id_hilo[i], NULL);
    }

    /*
        TODA ESTA PARTE ES PARA CREAR EL CSV DE RESULTADOS,
        se puede cambiar a una función aparte
    */
    // Abrimos el CSV
    FILE *csv = fopen("resultados_fourier.csv", "w");
    if (csv == NULL) {
        perror("Error al crear el CSV");
        return 1;
    }

    // Encabezados
    fprintf(csv, "x,ernesto,uriel,omar,diego\n");

    // Abrimos los 4 archivos de resultados
    FILE *archivos[4];

    for (int i = 0; i < 4; i++) {
        char nombre_archivo[50];
        snprintf( // Se utiliza snprintf para crear el nombre del archivo de resultados correspondiente a cada integrante del equipo. Se formatea la cadena de caracteres con el nombre del integrante y se guarda en la variable nombre_archivo.
            nombre_archivo,
            sizeof(nombre_archivo),
            "resultados_%s.txt",
            nombres[i]
        );
        archivos[i] = fopen(nombre_archivo, "r");
        if (archivos[i] == NULL) {
            perror("Error al abrir archivo de resultados");
            fclose(csv);
            return 1;
        }
    }

    // Leemos los 4 archivos y construimos el CSV
    for (int i = 0; i < COLUMNAS - 1; i++) {
        float resultados[4];
        for (int j = 0; j < 4; j++)
            fscanf(archivos[j], "%f", &resultados[j]);
        float x = -M_PI + (i * SALTO);
        fprintf(csv,"%f,%f,%f,%f,%f\n",x,resultados[0],resultados[1],resultados[2],resultados[3]);
    }

    fclose(csv);

    for (int i = 0; i < 4; i++)
        fclose(archivos[i]);

    /*
        TERMINA LA PARTE DE CREAR EL CSV DE RESULTADOS
    */
    return 0;
}