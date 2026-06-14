#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdio.h>

// TAD
typedef struct TabelaHash TabelaHash;

// Construtores / Destrutores
TabelaHash *createTabelaHash(long int tamanho);
void destroyTabelaHash(TabelaHash *self);

void insertTabelaHash(TabelaHash *self, long int codigo, long int nreg);
long int searchTabelaHash(TabelaHash *self, long int codigo);
long int getColisoesTabelaHash(TabelaHash *self);
void printParesTabelaHash(TabelaHash *self, FILE *output);

#endif
