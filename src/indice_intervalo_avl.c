#include "indice_intervalo_avl.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct NodeIntervaloAVL {
    double preco;
    long int nreg;
    int altura;
    struct NodeIntervaloAVL *esq;
    struct NodeIntervaloAVL *dir;
} NodeIntervaloAVL;
typedef struct IndiceIntervaloAVL {
    NodeIntervaloAVL *raiz;
    long int tamanho;
} IndiceIntervaloAVL;

// Funções da AVL
static int heightNode(NodeIntervaloAVL *node) {
    if (node == NULL) return 0;
    return node->altura;
}
static int fatorBalanceamento(NodeIntervaloAVL *node) {
    if (node == NULL) return 0;
    return heightNode(node->esq) - heightNode(node->dir);
}
static int maxHeight(int a, int b) {
    return (a > b) ? a : b;
}
static void updateHeight(NodeIntervaloAVL *node) {
    if (node != NULL) {
        node->altura = 1 + maxHeight(heightNode(node->esq), heightNode(node->dir));
    }
}
static NodeIntervaloAVL *rotateLeft(NodeIntervaloAVL *x) {
    NodeIntervaloAVL *y = x->dir;
    NodeIntervaloAVL *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}
static NodeIntervaloAVL *rotateRight(NodeIntervaloAVL *y) {
    NodeIntervaloAVL *x = y->esq;
    NodeIntervaloAVL *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}
static NodeIntervaloAVL *insertNode(NodeIntervaloAVL *node, double preco, long int nreg, long int *sizeOut) {
    if (node == NULL) {
        NodeIntervaloAVL *newNode = (NodeIntervaloAVL *)malloc(sizeof(NodeIntervaloAVL));
        if (newNode != NULL) {
            newNode->preco = preco;
            newNode->nreg = nreg;
            newNode->altura = 1;
            newNode->esq = NULL;
            newNode->dir = NULL;
            (*sizeOut)++;
        }
        return newNode;
    }
    if (preco < node->preco) {
        node->esq = insertNode(node->esq, preco, nreg, sizeOut);
    } else {
        node->dir = insertNode(node->dir, preco, nreg, sizeOut);
    }
    updateHeight(node);
    int balance = fatorBalanceamento(node);
    if (balance > 1 && preco < node->esq->preco) {
        return rotateRight(node);
    }
    if (balance < -1 && preco >= node->dir->preco) {
        return rotateLeft(node);
    }
    if (balance > 1 && preco >= node->esq->preco) {
        node->esq = rotateLeft(node->esq);
        return rotateRight(node);
    }
    if (balance < -1 && preco < node->dir->preco) {
        node->dir = rotateRight(node->dir);
        return rotateLeft(node);
    }
    return node;
}
IndiceIntervaloAVL *createIndiceIntervaloAVL(void) {
    IndiceIntervaloAVL *avl = (IndiceIntervaloAVL *)malloc(sizeof(IndiceIntervaloAVL));
    if (avl != NULL) {
        avl->raiz = NULL;
        avl->tamanho = 0;
    }
    return avl;
}
static void freeNode(NodeIntervaloAVL *node) {
    if (node != NULL) {
        freeNode(node->esq);
        freeNode(node->dir);
        free(node);
    }
}
void destroyIndiceIntervaloAVL(IndiceIntervaloAVL *self) {
    if (self != NULL) {
        freeNode(self->raiz);
        free(self);
    }
}
void insertIndiceIntervaloAVL(IndiceIntervaloAVL *self, double preco, long int nreg) {
    if (self != NULL) {
        self->raiz = insertNode(self->raiz, preco, nreg, &(self->tamanho));
    }
}
long int sizeIndiceIntervaloAVL(IndiceIntervaloAVL *self) {
    return (self != NULL) ? self->tamanho : 0;
}
long int heightIndiceIntervaloAVL(IndiceIntervaloAVL *self) {
    return (self != NULL) ? heightNode(self->raiz) : 0;
}
static void searchNode(NodeIntervaloAVL *node, double preco, Operador op, ListaLong *resultado) {
    if (node == NULL) return;

    int atende = 0;
    int explorarLeft = 1;
    int explorarRight = 1;
    switch (op) {
        case MAIOR:
            if (node->preco > preco) atende = 1;
            if (node->preco <= preco) explorarLeft = 0;
            break;
        case MAIOR_IGUAL:
            if (node->preco >= preco) atende = 1;
            if (node->preco < preco) explorarLeft = 0;
            break;
        case MENOR:
            if (node->preco < preco) atende = 1;
            if (node->preco >= preco) explorarRight = 0;
            break;
        case MENOR_IGUAL:
            if (node->preco <= preco) atende = 1;
            if (node->preco > preco) explorarRight = 0;
            break;
    }
    if (explorarLeft) {
        searchNode(node->esq, preco, op, resultado);
    }
    if (atende) {
        addListaLong(resultado, node->nreg);
    }
    if (explorarRight) {
        searchNode(node->dir, preco, op, resultado);
    }
}
void buscaIntervaloAVL(IndiceIntervaloAVL *self, double preco, Operador op, ListaLong *resultado) {
    if (self == NULL || resultado == NULL) return;
    searchNode(self->raiz, preco, op, resultado);
}
