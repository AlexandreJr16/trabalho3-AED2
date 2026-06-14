#ifndef PRODUTO_H
#define PRODUTO_H

#include <stdio.h>

// Produto: struct que define o layout de cada registro do arquivo binário.
// Conforme TADs.md: "Não é uma TAD com operações, é a struct que define
// o layout de cada registro do arquivo."
// A definição fica no .h para que arquivo.c possa usar sizeof(Produto)
// com fwrite/fread.
typedef struct {
    long int codigo;
    char nome[50];
    char categoria[20];
    double preco;
    int quantidade_estoque;
    long int codigo_fornecedor;
} Produto;

// Impressão (output == NULL → stdout)
void printProduto(Produto *self, FILE *output);

#endif // PRODUTO_H
