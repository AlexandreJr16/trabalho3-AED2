#ifndef INDICE_INTERVALO_AVL_H
#define INDICE_INTERVALO_AVL_H

#include "operador.h"
#include "lista.h"

// TAD 
typedef struct IndiceIntervaloAVL IndiceIntervaloAVL;

// Construtores / Destrutores
IndiceIntervaloAVL *createIndiceIntervaloAVL(void);
void destroyIndiceIntervaloAVL(IndiceIntervaloAVL *self);

void insertIndiceIntervaloAVL(IndiceIntervaloAVL *self, double preco, long int nreg);
long int sizeIndiceIntervaloAVL(IndiceIntervaloAVL *self);
long int heightIndiceIntervaloAVL(IndiceIntervaloAVL *self);

void buscaIntervaloAVL(IndiceIntervaloAVL *self, double preco, Operador op, ListaLong *resultado);

#endif // INDICE_INTERVALO_AVL_H
