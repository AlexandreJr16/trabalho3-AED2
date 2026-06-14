#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

// Gera um número aleatório em [min, max]
long int genRandomNumber(long int min, long int max);

// Sorteia `quantidade` valores distintos em [1, range]
void gerarValoresUnicos(long int *valores, long int quantidade, long int range);

// Retorna o tempo atual em segundos (clock_gettime CLOCK_MONOTONIC)
double getTempoAtual(void);

// Calcula a média de um vetor de tempos
double getMedia(double *tempos, long int quantidade);

// Calcula o desvio padrão de um vetor de tempos
double getDesvioPadrao(double *tempos, long int quantidade);

// Imprime média e desvio padrão dos tempos (output == NULL → stdout)
void showResults(double *tempos, long int quantidade, FILE *output);

#endif // UTILS_H
