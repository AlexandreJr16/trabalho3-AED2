#ifndef INDICE_BST_H
#define INDICE_BST_H

#include <stdio.h>

// TAD opaca para o índice BST
typedef struct IndiceBST IndiceBST;

// Construtores / Destrutores
IndiceBST *createIndiceBST(void);
void destroyIndiceBST(IndiceBST *self);

// Operações
void insertIndiceBST(IndiceBST *self, long int codigo, long int nreg);
long int searchIndiceBST(IndiceBST *self, long int codigo);
long int sizeIndiceBST(IndiceBST *self);
long int heightIndiceBST(IndiceBST *self);
void printParesIndiceBST(IndiceBST *self, FILE *output);

#endif // INDICE_BST_H
