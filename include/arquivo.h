#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "produto.h"

// TAD
typedef struct ArquivoProdutos ArquivoProdutos;

// Construtor / Destrutor
ArquivoProdutos *createArquivoProdutos(const char *caminho);
void destroyArquivoProdutos(ArquivoProdutos *self);

void gerarArquivoProdutos(ArquivoProdutos *self, long int quantidade, long int range_codigo);

int lerProdutoArquivo(ArquivoProdutos *self, long int nreg, Produto *out);

#include "operador.h"
#include "lista.h"

long int getQuantidadeArquivo(ArquivoProdutos *self);
long int buscaSequencialCodigo(ArquivoProdutos *self, long int codigo, Produto *out);
void buscaSequencialIntervalo(ArquivoProdutos *self, double preco, Operador op, ListaProdutos *resultado);

#endif 
