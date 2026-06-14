#ifndef PRODUTO_H
#define PRODUTO_H

#include <stdio.h>


typedef struct {
    long int codigo;
    char nome[50];
    char categoria[20];
    double preco;
    int quantidade_estoque;
    long int codigo_fornecedor;
} Produto;

void printProduto(Produto *self, FILE *output);

#endif