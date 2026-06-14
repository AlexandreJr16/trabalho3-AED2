#include "question.h"
#include "indice_intervalo_avl.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_REGISTROS_STDOUT 15

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

    fprintf(output, "\n##################################################\n");
    fprintf(output, "##   Question 2: Buscas por Intervalo (Preco)    ##\n");
    fprintf(output, "##################################################\n");

    long int qtd = getQuantidadeArquivo(ap);
    if (qtd < 30) {
        fprintf(output, "Erro: arquivo deve ter pelo menos 30 registros.\n");
        return;
    }

    //\Ler registros
    double *precosReais = (double *)malloc(qtd * sizeof(double));
    IndiceIntervaloAVL *avl = createIndiceIntervaloAVL();

    for (long int i = 0; i < qtd; i++) {
        Produto p;
        lerProdutoArquivo(ap, i, &p);
        precosReais[i] = p.preco;
        
        // AVL
        insertIndiceIntervaloAVL(avl, p.preco, i);
    }

    // Ordenar
    double *precosOrdenados = (double *)malloc(qtd * sizeof(double));
    memcpy(precosOrdenados, precosReais, qtd * sizeof(double));
    qsort(precosOrdenados, qtd, sizeof(double), cmpDouble);


    long int idxP2 = (long int)(qtd * 0.02);  
    long int idxP98 = (long int)(qtd * 0.98); 

    Operador operadores[30];
    for (int i = 0; i < 8; i++) operadores[i] = MAIOR;
    for (int i = 8; i < 15; i++) operadores[i] = MAIOR_IGUAL;
    for (int i = 15; i < 23; i++) operadores[i] = MENOR;
    for (int i = 23; i < 30; i++) operadores[i] = MENOR_IGUAL;

    // 30 alvos
    double alvos[30];
    for (int i = 0; i < 30; i++) {
        long int idx;
        if (operadores[i] == MAIOR || operadores[i] == MAIOR_IGUAL) {
            idx = idxP98 + (rand() % (qtd - idxP98));
        } else {
            idx = rand() % (idxP2 + 1);
        }
        alvos[i] = precosOrdenados[idx];
    }

    double temposSeq[30], temposAVL[30];

    //Medicao
    for (int i = 0; i < 30; i++) {
        double alvo = alvos[i];
        Operador op = operadores[i];
        double inicio, fim;

        // AVL
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

        //  Busca Seq
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

 //IMprime
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Resultados (Media e Desvio Padrao de 30 buscas):\n");
    fprintf(output, "--------------------------------------------------\n");
    fprintf(output, "Busca Seq. (Intervalo): Media = %f ms | Desvio = %f ms\n", mediaSeq, desvioSeq);
    fprintf(output, "Busca AVL  (Intervalo): Media = %f ms | Desvio = %f ms\n", mediaAVL, desvioAVL);
    fprintf(output, "##################################################\n");

    //Ietms achados
    {
        double alvoExemplo = alvos[29];
        Operador opExemplo = operadores[29];
        const char *opStr = (opExemplo == MAIOR ? ">" :
                             opExemplo == MAIOR_IGUAL ? ">=" :
                             opExemplo == MENOR ? "<" : "<=");

        ListaLong *nregsExemplo = createListaLong();
        buscaIntervaloAVL(avl, alvoExemplo, opExemplo, nregsExemplo);

        ListaProdutos *produtosExemplo = createListaProdutos();
        for (long int j = 0; j < getTamanhoListaLong(nregsExemplo); j++) {
            Produto p;
            lerProdutoArquivo(ap, getElementoListaLong(nregsExemplo, j), &p);
            addListaProdutos(produtosExemplo, p);
        }

        long int totalEncontrados = getTamanhoListaProdutos(produtosExemplo);

        fprintf(output, "\n########## REGISTROS ENCONTRADOS ##########\n");
        fprintf(output, "Consulta: preco %s %.2f\n", opStr, alvoExemplo);
        fprintf(output, "Total encontrados: %ld\n\n", totalEncontrados);

        if (output == stdout && totalEncontrados > MAX_REGISTROS_STDOUT) {
            // No terminal, imprimir apenas os primeiros registros
            for (long int j = 0; j < MAX_REGISTROS_STDOUT; j++) {
                Produto pj = getProdutoListaProdutos(produtosExemplo, j);
                printProduto(&pj, output);
            }
            fprintf(output, "  ... e mais %ld registros (ver arquivo completo em data/resultado_q2.txt)\n",
                    totalEncontrados - MAX_REGISTROS_STDOUT);
        } else {
            // No arquivo, imprimir todos
            printListaProdutos(produtosExemplo, output);
        }
        fprintf(output, "###########################################\n");

        destroyListaLong(nregsExemplo);
        destroyListaProdutos(produtosExemplo);
    }

    free(precosReais);
    free(precosOrdenados);
    destroyIndiceIntervaloAVL(avl);
}
