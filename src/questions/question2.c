#include "question.h"
#include "indice_intervalo_avl.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

static void calcularEstat(double tempos[], double *media, double *desvio) {
    double soma = 0.0;
    for (int i = 0; i < 30; i++) {
        soma += tempos[i];
    }
    *media = soma / 30.0;

    double somaVar = 0.0;
    for (int i = 0; i < 30; i++) {
        somaVar += (tempos[i] - *media) * (tempos[i] - *media);
    }
    *desvio = sqrt(somaVar / 30.0);
}

static int cmpDouble(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

void question2(ArquivoProdutos *ap, FILE *output) {
    if (ap == NULL) return;
    if (output == NULL) output = stdout;

    fprintf(output, "=== Iniciando Question 2: Buscas por Intervalo ===\n");

    long int qtd = getQuantidadeArquivo(ap);
    if (qtd < 30) {
        fprintf(output, "Erro: arquivo deve ter pelo menos 30 registros.\n");
        return;
    }

    // 1. Ler todos os registros e guardar os preços reais
    double *precosReais = (double *)malloc(qtd * sizeof(double));
    IndiceIntervaloAVL *avl = createIndiceIntervaloAVL();

    for (long int i = 0; i < qtd; i++) {
        Produto p;
        lerProdutoArquivo(ap, i, &p);
        precosReais[i] = p.preco;
        
        // 2. Construir Índice AVL
        insertIndiceIntervaloAVL(avl, p.preco, i);
    }

    // 3. Ordenar cópia dos preços para calcular percentis
    double *precosOrdenados = (double *)malloc(qtd * sizeof(double));
    memcpy(precosOrdenados, precosReais, qtd * sizeof(double));
    qsort(precosOrdenados, qtd, sizeof(double), cmpDouble);

    // Percentis para seletividade ~2%
    long int idxP2 = (long int)(qtd * 0.02);   // índice do percentil 2
    long int idxP98 = (long int)(qtd * 0.98);   // índice do percentil 98

    // Operadores cíclicos: 8 MAIOR, 7 MAIOR_IGUAL, 8 MENOR, 7 MENOR_IGUAL
    Operador operadores[30];
    for (int i = 0; i < 8; i++) operadores[i] = MAIOR;
    for (int i = 8; i < 15; i++) operadores[i] = MAIOR_IGUAL;
    for (int i = 15; i < 23; i++) operadores[i] = MENOR;
    for (int i = 23; i < 30; i++) operadores[i] = MENOR_IGUAL;

    // 4. Gerar 30 alvos seletivos baseados nos percentis
    double alvos[30];
    for (int i = 0; i < 30; i++) {
        long int idx;
        if (operadores[i] == MAIOR || operadores[i] == MAIOR_IGUAL) {
            // Sortear preço no percentil 98+ (retorna ~2% dos registros)
            idx = idxP98 + (rand() % (qtd - idxP98));
        } else {
            // Sortear preço no percentil 2- (retorna ~2% dos registros)
            idx = rand() % (idxP2 + 1);
        }
        alvos[i] = precosOrdenados[idx];
    }

    // Estruturas para guardar tempos
    double temposSeq[30], temposAVL[30];

    // 4. Loop de medição
    for (int i = 0; i < 30; i++) {
        double alvo = alvos[i];
        Operador op = operadores[i];
        double inicio, fim;

        // --- Busca AVL ---
        inicio = getTempoAtual();
        ListaLong *nregsEncontrados = createListaLong();
        buscaIntervaloAVL(avl, alvo, op, nregsEncontrados);

        ListaProdutos *produtosAVL = createListaProdutos();
        for (long int j = 0; j < getTamanhoListaLong(nregsEncontrados); j++) {
            Produto p;
            lerProdutoArquivo(ap, getElementoListaLong(nregsEncontrados, j), &p);
            addListaProdutos(produtosAVL, p);
        }
        fim = getTempoAtual();
        temposAVL[i] = (fim - inicio) * 1000.0;

        destroyListaLong(nregsEncontrados);
        destroyListaProdutos(produtosAVL);

        // --- Busca Sequencial Direta ---
        inicio = getTempoAtual();
        ListaProdutos *produtosSeq = createListaProdutos();
        buscaSequencialIntervalo(ap, alvo, op, produtosSeq);
        fim = getTempoAtual();
        temposSeq[i] = (fim - inicio) * 1000.0;

        destroyListaProdutos(produtosSeq);
    }

    double mediaSeq, desvioSeq;
    double mediaAVL, desvioAVL;

    calcularEstat(temposSeq, &mediaSeq, &desvioSeq);
    calcularEstat(temposAVL, &mediaAVL, &desvioAVL);

    // 5. Impressão de resultados
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Resultados (Media e Desvio Padrao de 30 buscas):\n");
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Busca Seq. (Intervalo): Media = %f ms | Desvio = %f ms\n", mediaSeq, desvioSeq);
    fprintf(output, "Busca AVL  (Intervalo): Media = %f ms | Desvio = %f ms\n", mediaAVL, desvioAVL);
    fprintf(output, "==================================================\n");

    // Limpeza
    free(precosReais);
    free(precosOrdenados);
    destroyIndiceIntervaloAVL(avl);
}
