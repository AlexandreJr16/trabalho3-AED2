#include "arquivo.h"
#include "question.h"
#include <stdio.h>
#include <stdlib.h>

// Parâmetros do dataset pequeno (Dica 2)
#define DICA2_QTD       150
#define DICA2_RANGE     1500

// Parâmetros do dataset grande (experimentos)
#define EXP_QTD         100000
#define EXP_RANGE       1000000

int main(void) {
    // Seed fixa para reprodutibilidade entre execuções.
    // Garante que os mesmos arquivos, alvos e colisões são gerados sempre.
    // Para resultados não-determinísticos, usar srand(time(NULL)).
    srand(42);

    // ==================== DICA 2 ====================
    ArquivoProdutos *ap_pequeno = createArquivoProdutos("data/produtos_pequeno.dat");
    gerarArquivoProdutos(ap_pequeno, DICA2_QTD, DICA2_RANGE);

    question0(ap_pequeno, stdout);

    FILE *fq0 = fopen("data/resultado_dica2.txt", "w");
    if (fq0 != NULL) {
        question0(ap_pequeno, fq0);
        fclose(fq0);
        printf("  Output completo salvo em 'data/resultado_dica2.txt'\n");
    }

    destroyArquivoProdutos(ap_pequeno);

    // ==================== EXPERIMENTOS ====================
    printf("\n##################################################\n");
    printf("##        EXPERIMENTOS: Dataset Grande           ##\n");
    printf("##################################################\n");
    ArquivoProdutos *ap_grande = createArquivoProdutos("data/produtos_grande.dat");
    gerarArquivoProdutos(ap_grande, EXP_QTD, EXP_RANGE);

    // Question 1
    question1(ap_grande, stdout);

    FILE *fq1 = fopen("data/resultado_q1.txt", "w");
    if (fq1 != NULL) {
        question1(ap_grande, fq1);
        fclose(fq1);
    }

    // Question 2
    question2(ap_grande, stdout);

    FILE *fq2 = fopen("data/resultado_q2.txt", "w");
    if (fq2 != NULL) {
        question2(ap_grande, fq2);
        fclose(fq2);
    }

    destroyArquivoProdutos(ap_grande);
    printf("\n##################################################\n");
    printf("##       TODOS OS EXPERIMENTOS FINALIZADOS       ##\n");
    printf("##################################################\n");
    return 0;
}
