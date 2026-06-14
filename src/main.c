#include "arquivo.h"
#include "question.h"
#include <stdio.h>
#include <stdlib.h>

// Parâmetros do dataset grande (experimentos)
#define EXP_QTD         100000
#define EXP_RANGE       1000000

int main(void) {
   
    srand(67);

    // EXPERIMENTOS
    printf("EXPERIMENTOS: Dataset Grande");
    ArquivoProdutos *ap_grande = createArquivoProdutos("data/produtos_grande.dat");
    gerarArquivoProdutos(ap_grande, EXP_QTD, EXP_RANGE);

    // Questão 1
    question1(ap_grande, stdout);
    FILE *fq1 = fopen("data/resultado_q1.txt", "w");
    if (fq1 != NULL) {
        question1(ap_grande, fq1);
        fclose(fq1);
    }

    // Questão 2
    question2(ap_grande, stdout);
    FILE *fq2 = fopen("data/resultado_q2.txt", "w");
    if (fq2 != NULL) {
        question2(ap_grande, fq2);
        fclose(fq2);
    }
    // FIM DOS EXPERIMENTOS


    destroyArquivoProdutos(ap_grande);

    return 0;
}
