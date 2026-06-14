#include "produto.h"
#include <stdlib.h>
#include <string.h>

void printProduto(Produto *self, FILE *output) {
    if (output == NULL) {
        output = stdout;
    }
    fprintf(output, "Codigo: %ld | Nome: %s | Categoria: %s | Preco: %.2f | Estoque: %d | Fornecedor: %ld\n",
            self->codigo, self->nome, self->categoria, self->preco,
            self->quantidade_estoque, self->codigo_fornecedor);
}
