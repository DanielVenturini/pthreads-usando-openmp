#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

// Entrada e saída.
float *h_a;
float *h_b;
float *h_c;

int partition = 0;
int last_assigned = 0;
int total_of_iterations = 0;
bool work_finished = false;

sem_t mutex;

void init_array(int n) {

  	h_a = (float *) malloc(n * sizeof(float));
  	h_b = (float *) malloc(n * sizeof(float));
  	h_c = (float *) malloc(n * sizeof(float));

  	//fprintf(stdout, "Thread[%lu]: Initializing the arrays.\n", (long int) id_omp);
  	int i;
  // Initialize vectors on host.
	for (i = 0; i < n; i++) {
	  	h_a[i] = 0.5;
	  	h_b[i] = 0.5;
	}
}

void print_array(int n) {
  	int i;
  	for (i = 0; i < n; i++) {
    	//fprintf(stdout, "Thread[%lu]: h_c[%07d]: %f.\n", id_omp, i, h_c[i]);
    	fprintf(stdout, "h_c[%07d]: %f.\n", i, h_c[i]);
  	}
}

void check_result(int n){
  // Soma dos elementos do array C e divide por N, o valor deve ser igual a 1.
  	int i;
  	float sum = 0;
  	//fprintf(stdout, "Thread[%lu]: Checking.\n", (long int) id_omp);
  
  	for (i = 0; i < n; i++) {
	  	sum += h_c[i];
		}
	
  	//fprintf(stdout, "Thread[%lu]: Final Result: (%f, %f).\n", (long int) id_omp, sum, (float)(sum / (float)n));
}

bool get_next_loop_partition(int *ii, int *ff, int id_omp) {
  
  	bool ret = true;

  	*ii = 0;
  	*ff = 0;

  	//fprintf(stdout, "Thread[%lu]: Trying to get the next partition.\n", id_omp);

  	sem_wait(&mutex);
  
  	if(work_finished){
    	ret = false;
  	} else{
    	*ii = last_assigned;
    	*ff = last_assigned + partition;

    if (*ff > total_of_iterations){
      	*ff = total_of_iterations;
    }

   	last_assigned = *ff;
    ret = true;

    work_finished = (last_assigned == total_of_iterations);
  }

  sem_post(&mutex);

  //fprintf(stdout, "Thread[%lu]: Got the partition [%lu, %lu].\n", id_omp, *ii, *ff);

  return ret;
}


int main(int argc, char *argv[]) {
  	
  	int i, num_elements, num_threads = 0;

  	if(argc < 4){
    	//fprintf(stderr, "Uso: %s <num_elements> <num_threads> <partition_size>\n", argv[0]);
    	exit(0);
  	}

  	num_elements = atoi(argv[1]);
  	num_threads = atoi(argv[2]);
  	partition = atoi(argv[3]);

  	total_of_iterations = num_elements;

  	//fprintf(stdout, "Thread[%lu]: num_elements: %d num_threads: %d.\n", (long int) id_omp, num_elements, num_threads);
  	//fprintf(stdout, "Thread[%lu]: Allocating the arrays.\n", (long int) id_omp);

  	init_array(num_elements);
  	sem_init(&mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
                                 /* second param = 0 - semaphore is local */

  	work_finished = false;

  	#pragma omp parallel num_threads(num_threads) default(none) shared(work_finished, h_a, h_b, h_c)
  	{

  		int id_omp = omp_get_thread_num () ;
		int i, ii, ff;

		bool have_work = false;

		while ((have_work = get_next_loop_partition(&ii, &ff, id_omp)) != false){
			//fprintf(stdout, "Thread[%lu,%lu]: Working on partition: [%d..%d]: %d.\n", id_omp, id_omp, ii, ff, (ff - ii));
		    for (i = ii; i < ff; i++) {
		    	h_c[i] = sin(h_a[i]) + sqrt(cos(h_b[i]));
		    }
		}

		if(!have_work){
		    //fprintf(stdout, "  Thread[%lu]: No work to do.\n", id_omp);
		}
		//fprintf(stdout, "  Thread[%lu]: Exiting.\n", id_omp);

  	}
 
	check_result(num_elements);
  	return 0;
}
