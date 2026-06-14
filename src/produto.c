#include "produto.h"
#include <stdlib.h>
#include <string.h>

Produto *createProduto(long int codigo, const char *nome, const char *categoria,
                       double preco, int quantidade_estoque, long int codigo_fornecedor) {
    Produto *p = (Produto *)malloc(sizeof(Produto));
    if (p == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para Produto\n");
        exit(EXIT_FAILURE);
    }

    p->codigo = codigo;
    strncpy(p->nome, nome, sizeof(p->nome) - 1);
    p->nome[sizeof(p->nome) - 1] = '\0';
    strncpy(p->categoria, categoria, sizeof(p->categoria) - 1);
    p->categoria[sizeof(p->categoria) - 1] = '\0';
    p->preco = preco;
    p->quantidade_estoque = quantidade_estoque;
    p->codigo_fornecedor = codigo_fornecedor;

    return p;
}

void destroyProduto(Produto *self) {
    free(self);
}

void printProduto(Produto *self, FILE *output) {
    if (output == NULL) {
        output = stdout;
    }
    fprintf(output, "Codigo: %ld | Nome: %s | Categoria: %s | Preco: %.2f | Estoque: %d | Fornecedor: %ld\n",
            self->codigo, self->nome, self->categoria, self->preco,
            self->quantidade_estoque, self->codigo_fornecedor);
}
