===================================================
| Paradigma de Programação Paralela e Distribuida |
===================================================

Implementação de região paralela para resolver o problema de soma de vetor usando threads. Foi usado o OpenMP - uma interface para a programação multi-processo de memória compartilhada em múltiplas plataformas - para substituir a utilização do LPthreads.

Usando algumas funções da biblioteca math.h, então por isso se passa no parametro o "-lm".

compile:
	
	dinamico
	gcc -fopenmp vectoradd-openmp-dinamico.c -o vectoradd-openmp-dinamico -lm

	gcc -fopenmp vectoradd-openmp-estatico.c -o vectoradd-openmp-estatico -lm

run:

	dinamico:
	./vectoradd-openmp-dinamico <num_elements> <num_threads> <partition_size>

	estatico:
	./vectoradd-openmp-estatico Uso: <num_elements> <numthreads>

