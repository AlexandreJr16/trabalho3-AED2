#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include "produto.h"

//ListaProdutos

typedef struct ListaProdutos ListaProdutos;

ListaProdutos *createListaProdutos(void);
void destroyListaProdutos(ListaProdutos *self);

void addListaProdutos(ListaProdutos *self, Produto p);
long int getTamanhoListaProdutos(ListaProdutos *self);
Produto getProdutoListaProdutos(ListaProdutos *self, long int indice);
void printListaProdutos(ListaProdutos *self, FILE *output);

//ListaLong

typedef struct ListaLong ListaLong;

ListaLong *createListaLong(void);
void destroyListaLong(ListaLong *self);

void addListaLong(ListaLong *self, long int valor);
long int getTamanhoListaLong(ListaLong *self);
long int getElementoListaLong(ListaLong *self, long int indice);

#endif
