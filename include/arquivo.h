#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "produto.h"

// TAD Opaca — struct definida em arquivo.c
typedef struct ArquivoProdutos ArquivoProdutos;

// Construtor / Destrutor
ArquivoProdutos *createArquivoProdutos(const char *caminho);
void destroyArquivoProdutos(ArquivoProdutos *self);

// Gera `quantidade` produtos com codigos únicos em [1, range_codigo]
// e grava no arquivo binário (sobrescrevendo). Atualiza `quantidade`.
void gerarArquivoProdutos(ArquivoProdutos *self, long int quantidade, long int range_codigo);

// Lê o registro na posição `nreg` do arquivo para `out`.
// Retorna 1 se nreg é válido, 0 caso contrário.
int lerProdutoArquivo(ArquivoProdutos *self, long int nreg, Produto *out);

#include "operador.h"
#include "lista.h"

// Retorna o número de registros no arquivo
long int getQuantidadeArquivo(ArquivoProdutos *self);
long int buscaSequencialCodigo(ArquivoProdutos *self, long int codigo, Produto *out);
void buscaSequencialIntervalo(ArquivoProdutos *self, double preco, Operador op, ListaProdutos *resultado);

#endif // ARQUIVO_H
