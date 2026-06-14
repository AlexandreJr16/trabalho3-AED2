#include "question.h"
#include "indice_bst.h"
#include "tabela_hash.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>

#define REPETICOES_Q1 10000

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

void question1(ArquivoProdutos *ap, FILE *output) {
    if (ap == NULL) return;
    if (output == NULL) output = stdout;

    fprintf(output, "\n##################################################\n");
    fprintf(output, "##   Question 1: Avaliacao de Desempenho (Chave) ##\n");
    fprintf(output, "##################################################\n");

    long int qtd = getQuantidadeArquivo(ap);
    if (qtd < 30) {
        fprintf(output, "Erro: arquivo deve ter pelo menos 30 registros.\n");
        return;
    }

    // 1. Ler todos os registros e guardar codigos
    long int *codigosReais = (long int *)malloc(qtd * sizeof(long int));
    IndiceBST *bst = createIndiceBST();
    TabelaHash *hash = createTabelaHash(200003); // Load factor ~0.5

    for (long int i = 0; i < qtd; i++) {
        Produto p;
        lerProdutoArquivo(ap, i, &p);
        codigosReais[i] = p.codigo;
        
        // 2. Construir Índices
        insertIndiceBST(bst, p.codigo, i);
        insertTabelaHash(hash, p.codigo, i);
    }

    // 3. Sortear 30 índices do array de codigos
    long int indicesSorteados[30];
    gerarValoresUnicos(indicesSorteados, 30, qtd);
    
    long int alvos[30];
    for (int i = 0; i < 30; i++) {
        indicesSorteados[i] -= 1; // convert from [1, qtd] to [0, qtd-1]
        alvos[i] = codigosReais[indicesSorteados[i]];
    }

    // Estruturas para guardar tempos
    double temposSeq[30], temposBST[30], temposHash[30];

    // 4. Loop de medição com repetições para confiabilidade
    //    Cada tempo = pipeline completo (índice + acesso a disco)
    for (int i = 0; i < 30; i++) {
        long int alvo = alvos[i];
        double inicio, fim;
        Produto p;

        // --- Busca Sequencial (10 repetições para evitar lentidão extrema de I/O) ---
        inicio = getTempoAtual();
        for (int r = 0; r < 10; r++) {
            buscaSequencialCodigo(ap, alvo, &p);
        }
        fim = getTempoAtual();
        temposSeq[i] = ((fim - inicio) * 1000.0) / 10.0;

        // --- Busca IndiceBST (pipeline completo: índice + acesso a disco) ---
        inicio = getTempoAtual();
        for (int r = 0; r < REPETICOES_Q1; r++) {
            long int nreg = searchIndiceBST(bst, alvo);
            if (nreg >= 0) lerProdutoArquivo(ap, nreg, &p);
        }
        fim = getTempoAtual();
        temposBST[i] = ((fim - inicio) * 1000.0) / REPETICOES_Q1;

        // --- Busca TabelaHash (pipeline completo: índice + acesso a disco) ---
        inicio = getTempoAtual();
        for (int r = 0; r < REPETICOES_Q1; r++) {
            long int nreg = searchTabelaHash(hash, alvo);
            if (nreg >= 0) lerProdutoArquivo(ap, nreg, &p);
        }
        fim = getTempoAtual();
        temposHash[i] = ((fim - inicio) * 1000.0) / REPETICOES_Q1;
    }

    double mediaSeq, desvioSeq;
    double mediaBST, desvioBST;
    double mediaHash, desvioHash;

    calcularEstat(temposSeq, &mediaSeq, &desvioSeq);
    calcularEstat(temposBST, &mediaBST, &desvioBST);
    calcularEstat(temposHash, &mediaHash, &desvioHash);

    // 5 e 6. Impressão de resultados
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Resultados (Media e Desvio Padrao de 30 buscas):\n");
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Busca Sequencial : Media = %f ms | Desvio = %f ms\n", mediaSeq, desvioSeq);
    fprintf(output, "Busca Indice BST : Media = %f ms | Desvio = %f ms\n", mediaBST, desvioBST);
    fprintf(output, "Busca Tabela Hash: Media = %f ms | Desvio = %f ms\n", mediaHash, desvioHash);
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Colisoes na Tabela Hash (tam 200003): %ld\n", getColisoesTabelaHash(hash));
    fprintf(output, "##################################################\n");

    // Limpeza
    free(codigosReais);
    destroyIndiceBST(bst);
    destroyTabelaHash(hash);
}
