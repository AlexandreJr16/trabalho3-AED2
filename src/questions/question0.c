#include "question.h"
#include "indice_bst.h"
#include "tabela_hash.h"
#include <assert.h>
#include <stdio.h>

#define DICA2_HASH_TAM 151

void question0(ArquivoProdutos *ap, FILE *output) {
    if (ap == NULL) return;
    if (output == NULL) output = stdout;

    fprintf(output, "##################################################\n");
    fprintf(output, "##   Dica 2: Validacao de Indices (Dataset Pequeno)\n");
    fprintf(output, "##################################################\n");

    long int qtd = getQuantidadeArquivo(ap);

    // 1. Construir índices BST e Hash a partir do arquivo
    IndiceBST *bst = createIndiceBST();
    TabelaHash *hash = createTabelaHash(DICA2_HASH_TAM);

    for (long int i = 0; i < qtd; i++) {
        Produto p;
        if (lerProdutoArquivo(ap, i, &p)) {
            insertIndiceBST(bst, p.codigo, i);
            insertTabelaHash(hash, p.codigo, i);
        }
    }

    // 2. Validar: cada registro deve ser encontrado corretamente em ambos os índices
    for (long int i = 0; i < qtd; i++) {
        Produto p;
        if (lerProdutoArquivo(ap, i, &p)) {
            long int nregBST = searchIndiceBST(bst, p.codigo);
            long int nregHash = searchTabelaHash(hash, p.codigo);
            assert(nregBST == i);
            assert(nregHash == i);
        }
    }
    fprintf(output, "  Todos os asserts passaram com sucesso!\n");

    // 3. Imprimir pares <chave, nreg> de cada índice
    fprintf(output, "\n--- Pares na BST (in-order) ---\n");
    printParesIndiceBST(bst, output);
    fprintf(output, "\n--- Pares na Tabela Hash (por bucket) ---\n");
    printParesTabelaHash(hash, output);
    fprintf(output, "\n--- Colisoes na Tabela Hash (tam %d): %ld ---\n",
            DICA2_HASH_TAM, getColisoesTabelaHash(hash));
    fprintf(output, "##################################################\n");

    // Limpeza
    destroyIndiceBST(bst);
    destroyTabelaHash(hash);
}
