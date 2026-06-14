#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

long int genRandomNumber(long int min, long int max);
void gerarValoresUnicos(long int *valores, long int quantidade, long int range);
double getTempoAtual(void);
double getMedia(double *tempos, long int quantidade);
double getDesvioPadrao(double *tempos, long int quantidade);
void showResults(double *tempos, long int quantidade, FILE *output);

#endif 
