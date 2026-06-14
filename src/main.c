#include "arquivo.h"
#include "indice_bst.h"
#include "indice_intervalo_avl.h"
#include "question.h"
#include "tabela_hash.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void runDica2(void) {
  printf("=== Dica 2: Validacao de Dataset Pequeno ===\n");
  const char *filepath = "data/produtos_pequeno.dat";
  long int qtd = 150;
  long int maxCodigo = 1500;

  ArquivoProdutos *ap = createArquivoProdutos(filepath);
  gerarArquivoProdutos(ap, qtd, maxCodigo);

  IndiceBST *bst = createIndiceBST();
  // A Dica 2 pede Tabela Hash de tamanho 151
  TabelaHash *hash = createTabelaHash(151);

  for (long int i = 0; i < qtd; i++) {
    Produto p;
    if (lerProdutoArquivo(ap, i, &p)) {
      insertIndiceBST(bst, p.codigo, i);
      insertTabelaHash(hash, p.codigo, i);
    }
  }

  // Validar com assert
  for (long int i = 0; i < qtd; i++) {
    Produto p;
    if (lerProdutoArquivo(ap, i, &p)) {
      long int nregBST = searchIndiceBST(bst, p.codigo);
      long int nregHash = searchTabelaHash(hash, p.codigo);
      assert(nregBST == i);
      assert(nregHash == i);
    }
  }
  printf("  Asserts da Dica 2 passaram com sucesso!\n");

  FILE *fout = fopen("data/dica2.txt", "w");
  if (fout != NULL) {
    fprintf(fout, "--- Pares na BST ---\n");
    printParesIndiceBST(bst, fout);
    fprintf(fout, "\n--- Pares na Tabela Hash ---\n");
    printParesTabelaHash(hash, fout);
    fclose(fout);
    printf("  Output gerado em 'data/dica2.txt'\n");
  }

  destroyIndiceBST(bst);
  destroyTabelaHash(hash);
  destroyArquivoProdutos(ap);
}

void runExperiments(void) {
  printf("\n=== Experimentos: Dataset Grande ===\n");
  const char *filepath = "data/produtos_grande.dat";
  long int qtd = 100000;
  long int maxCodigo = 1000000;

  ArquivoProdutos *ap = createArquivoProdutos(filepath);
  gerarArquivoProdutos(ap, qtd, maxCodigo);

  // Question 1
  printf("\n>>> QUESTION 1 (Executando para stdout e depois para "
         "data/resultado_q1.txt)...\n");
  // Para o stdout:
  question1(ap, stdout);

  FILE *fq1 = fopen("data/resultado_q1.txt", "w");
  if (fq1 != NULL) {
    question1(ap, fq1);
    fclose(fq1);
  }

  // Question 2
  printf("\n>>> QUESTION 2 (Executando para stdout e depois para "
         "data/resultado_q2.txt)...\n");
  // Para o stdout:
  question2(ap, stdout);

  FILE *fq2 = fopen("data/resultado_q2.txt", "w");
  if (fq2 != NULL) {
    question2(ap, fq2);
    fclose(fq2);
  }

  destroyArquivoProdutos(ap);
  printf("\n=== TODOS OS EXPERIMENTOS FINALIZADOS ===\n");
}

int main(void) {
  runDica2();
  runExperiments();
  return 0;
}
