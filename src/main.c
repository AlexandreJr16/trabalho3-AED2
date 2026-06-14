#include "arquivo.h"
#include "question.h"
#include <stdio.h>
#include <stdlib.h>

// // Parâmetros do dataset pequeno (Dica 2)
// #define PEQUENO_QTD       150
// #define PEQUENO_RANGE     1500

// Parâmetros do dataset grande (experimentos)
#define EXP_QTD         100000
#define EXP_RANGE       1000000

int main(void) {
   
    srand(67);

    // // TESTE PEQUENO
    // ArquivoProdutos *ap_pequeno = createArquivoProdutos("data/produtos_pequeno.dat");
    // gerarArquivoProdutos(ap_pequeno, PEQUENO_QTD, PEQUENO_RANGE);
    // question0(ap_pequeno, stdout);
    // FILE *fq0 = fopen("data/resultado_pequeno.txt", "w");
    // if (fq0 != NULL) {
    //     question0(ap_pequeno, fq0);
    //     fclose(fq0);
    //     printf("  Output completo salvo em 'data/resultado_dica2.txt'\n");
    // }
    // destroyArquivoProdutos(ap_pequeno);
    // // FIM DO TESTE PEQUENO


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
