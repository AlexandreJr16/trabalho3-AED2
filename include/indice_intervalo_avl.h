#ifndef INDICE_INTERVALO_AVL_H
#define INDICE_INTERVALO_AVL_H

#include "operador.h"
#include "lista.h"

// TAD opaca para o índice AVL
typedef struct IndiceIntervaloAVL IndiceIntervaloAVL;

// Construtores / Destrutores
IndiceIntervaloAVL *createIndiceIntervaloAVL(void);
void destroyIndiceIntervaloAVL(IndiceIntervaloAVL *self);

// Inserção
void insertIndiceIntervaloAVL(IndiceIntervaloAVL *self, double preco, long int nreg);

// Operações de consulta e metadados
long int sizeIndiceIntervaloAVL(IndiceIntervaloAVL *self);
long int heightIndiceIntervaloAVL(IndiceIntervaloAVL *self);

// Busca por intervalo com preenchimento da ListaLong (resultado)
void buscaIntervaloAVL(IndiceIntervaloAVL *self, double preco, Operador op, ListaLong *resultado);

#endif // INDICE_INTERVALO_AVL_H
