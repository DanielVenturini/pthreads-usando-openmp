#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Entrada e saída.
float *h_a;
float *h_b;
float *h_c;

int partition = 0;
int n = 0;

void init_array(int n) {

    fprintf(stdout, "Inicializando os arrays.\n");

    h_a = (float *) malloc(n * sizeof(float));
    h_b = (float *) malloc(n * sizeof(float));
    h_c = (float *) malloc(n * sizeof(float));

    int i;
    // Initialize vectors on host.
	for (i = 0; i < n; i++) {
	    h_a[i] = 5;
	    h_b[i] = 9;
	}
}

void print_array(int n) {

    int i;
    for (i = 0; i < n; i++) {
	    fprintf(stdout, "h_c[%07d]: %f\n", i, h_c[i]);
    }
}

void check_result(int n){
    // Soma dos elementos do array C e divide por N, o valor deve ser igual a 1.
    int i;
    float sum = 0;
    fprintf(stdout, "Verificando o resultado.\n");

    for (i = 0; i < n; i++) {
	    sum += h_c[i];
	}

	fprintf(stdout, "Resultado Final: (%f, %f)\n", sum, (float)(sum / (float)n));
}

int main(int argc, char *argv[]) {

    int i, num_elementos, num_threads = 0;

    if(argc < 3){
        printf("Uso: %s <size> <numthreads>\n", argv[0]);
        exit(0);
    }

    num_elementos = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    partition = num_elementos / num_threads;
    n = num_elementos;

    printf("num_elementos: %d num_threads: %d\n", num_elementos, num_threads);

    init_array(num_elementos);

    #pragma omp parallel num_threads(num_threads) default(none) shared(num_elementos, partition, n, h_a, h_b, h_c)
    {

        int id_thread = omp_get_thread_num();
        int ii = id_thread * partition;
        int jj = ii + partition > n ? n : ii+partition;

        int i;
        for(i = ii; i < jj; i ++){
            h_c[i] = sin(h_a[i]) + sqrt(cos(h_b[i]));

        }

    }


    printf("Thread[%lu]: Todas as threads terminaram...\n", (long int) pthread_self());
    printf("Thread[%lu]: Imprimindo o resultado.\n", (long int) pthread_self());
    printf("Thread[%lu]: Verificando o resultado.\n", (long int) pthread_self());
	//check_result(num_elementos);

    printf("Thread[%lu]: Fui, Tchau!\n", (long int) pthread_self());

  return 0;
}
