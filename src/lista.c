#include "lista.h"
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 8

// ======== ListaProdutos ========

struct ListaProdutos {
    Produto *dados;
    long int tamanho;
    long int capacidade;
};

ListaProdutos *createListaProdutos(void) {
    ListaProdutos *lista = (ListaProdutos *)malloc(sizeof(ListaProdutos));
    if (lista == NULL) {
        fprintf(stderr, "Erro: falha ao alocar ListaProdutos\n");
        exit(EXIT_FAILURE);
    }

    lista->dados = (Produto *)malloc(CAPACIDADE_INICIAL * sizeof(Produto));
    if (lista->dados == NULL) {
        fprintf(stderr, "Erro: falha ao alocar vetor de ListaProdutos\n");
        free(lista);
        exit(EXIT_FAILURE);
    }

    lista->tamanho = 0;
    lista->capacidade = CAPACIDADE_INICIAL;
    return lista;
}

void destroyListaProdutos(ListaProdutos *self) {
    if (self != NULL) {
        free(self->dados);
        free(self);
    }
}

void addListaProdutos(ListaProdutos *self, Produto p) {
    if (self->tamanho == self->capacidade) {
        self->capacidade *= 2;
        self->dados = (Produto *)realloc(self->dados, self->capacidade * sizeof(Produto));
        if (self->dados == NULL) {
            fprintf(stderr, "Erro: falha ao realocar ListaProdutos\n");
            exit(EXIT_FAILURE);
        }
    }
    self->dados[self->tamanho] = p;
    self->tamanho++;
}

long int getTamanhoListaProdutos(ListaProdutos *self) {
    return self->tamanho;
}

Produto getProdutoListaProdutos(ListaProdutos *self, long int indice) {
    return self->dados[indice];
}

void printListaProdutos(ListaProdutos *self, FILE *output) {
    if (output == NULL) {
        output = stdout;
    }
    for (long int i = 0; i < self->tamanho; i++) {
        printProduto(&self->dados[i], output);
    }
}

// ======== ListaLong ========

struct ListaLong {
    long int *dados;
    long int tamanho;
    long int capacidade;
};

ListaLong *createListaLong(void) {
    ListaLong *lista = (ListaLong *)malloc(sizeof(ListaLong));
    if (lista == NULL) {
        fprintf(stderr, "Erro: falha ao alocar ListaLong\n");
        exit(EXIT_FAILURE);
    }

    lista->dados = (long int *)malloc(CAPACIDADE_INICIAL * sizeof(long int));
    if (lista->dados == NULL) {
        fprintf(stderr, "Erro: falha ao alocar vetor de ListaLong\n");
        free(lista);
        exit(EXIT_FAILURE);
    }

    lista->tamanho = 0;
    lista->capacidade = CAPACIDADE_INICIAL;
    return lista;
}

void destroyListaLong(ListaLong *self) {
    if (self != NULL) {
        free(self->dados);
        free(self);
    }
}

void addListaLong(ListaLong *self, long int valor) {
    if (self->tamanho == self->capacidade) {
        self->capacidade *= 2;
        self->dados = (long int *)realloc(self->dados, self->capacidade * sizeof(long int));
        if (self->dados == NULL) {
            fprintf(stderr, "Erro: falha ao realocar ListaLong\n");
            exit(EXIT_FAILURE);
        }
    }
    self->dados[self->tamanho] = valor;
    self->tamanho++;
}

long int getTamanhoListaLong(ListaLong *self) {
    return self->tamanho;
}

long int getElementoListaLong(ListaLong *self, long int indice) {
    return self->dados[indice];
}
