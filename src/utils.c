#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

long int genRandomNumber(long int min, long int max) {
    return min + rand() % (max - min + 1);
}

void gerarValoresUnicos(long int *valores, long int quantidade, long int range) {
    // Usar um vetor de flags para garantir unicidade
    // range pode ser grande (até 1.000.000), mas cabe na memória
    char *usado = (char *)calloc(range + 1, sizeof(char));
    if (usado == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória em gerarValoresUnicos\n");
        exit(EXIT_FAILURE);
    }

    long int gerados = 0;
    while (gerados < quantidade) {
        long int v = genRandomNumber(1, range);
        if (!usado[v]) {
            usado[v] = 1;
            valores[gerados] = v;
            gerados++;
        }
    }

    free(usado);
}

double getTempoAtual(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

double getMedia(double *tempos, long int quantidade) {
    double soma = 0.0;
    for (long int i = 0; i < quantidade; i++) {
        soma += tempos[i];
    }
    return soma / (double)quantidade;
}

double getDesvioPadrao(double *tempos, long int quantidade) {
    double media = getMedia(tempos, quantidade);
    double soma = 0.0;
    for (long int i = 0; i < quantidade; i++) {
        double diff = tempos[i] - media;
        soma += diff * diff;
    }
    return sqrt(soma / (double)quantidade);
}

void showResults(double *tempos, long int quantidade, FILE *output) {
    if (output == NULL) {
        output = stdout;
    }
    double media = getMedia(tempos, quantidade);
    double desvio = getDesvioPadrao(tempos, quantidade);
    fprintf(output, "  Media: %.9f s\n", media);
    fprintf(output, "  Desvio Padrao: %.9f s\n", desvio);
}
