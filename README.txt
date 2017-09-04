===================================================
| Paradigma de Programação Paralela e Distribuida |
===================================================

Implementação de região paralela para resolver o problema de soma de vetor usando threads. Foi usado o OpenMP - uma interface para a programação multi-processo de memória compartilhada em múltiplas plataformas - para substituir a utilização do LPthreads.

compile:
	
	dinamico
	gcc -lpthread vectoradd-openmp-dinamico.c -o vectoradd-openmp-dinamico

	gcc -lpthread vectoradd-openmp-estatico.c -o vectoradd-openmp-estatico

run:

	dinamico:
	./vectoradd-openmp-dinamico <num_elements> <num_threads> <partition_size>

	estatico:
	./vectoradd-openmp-estatico Uso: <num_elements> <numthreads>

