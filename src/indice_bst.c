#include "indice_bst.h"
#include <stdlib.h>

typedef struct NodeIndiceBST {
    long int codigo;
    long int nreg;
    struct NodeIndiceBST *esq;
    struct NodeIndiceBST *dir;
} NodeIndiceBST;

struct IndiceBST {
    NodeIndiceBST *raiz;
    long int tamanho;
};

// Funções privadas
static NodeIndiceBST *createNodeIndiceBST(long int codigo, long int nreg) {
    NodeIndiceBST *node = (NodeIndiceBST *)malloc(sizeof(NodeIndiceBST));
    if (node == NULL) {
        fprintf(stderr, "Erro: falha ao alocar NodeIndiceBST\n");
        exit(EXIT_FAILURE);
    }
    node->codigo = codigo;
    node->nreg = nreg;
    node->esq = NULL;
    node->dir = NULL;
    return node;
}

static void destroyNode(NodeIndiceBST *node) {
    if (node != NULL) {
        destroyNode(node->esq);
        destroyNode(node->dir);
        free(node);
    }
}

static NodeIndiceBST *insertNode(NodeIndiceBST *node, long int codigo, long int nreg, int *inserido) {
    if (node == NULL) {
        *inserido = 1;
        return createNodeIndiceBST(codigo, nreg);
    }

    if (codigo < node->codigo) {
        node->esq = insertNode(node->esq, codigo, nreg, inserido);
    } else if (codigo > node->codigo) {
        node->dir = insertNode(node->dir, codigo, nreg, inserido);
    }    
    return node;
}

static NodeIndiceBST *searchNode(NodeIndiceBST *node, long int codigo) {
    if (node == NULL) {
        return NULL;
    }
    if (codigo < node->codigo) {
        return searchNode(node->esq, codigo);
    } else if (codigo > node->codigo) {
        return searchNode(node->dir, codigo);
    } else {
        return node;
    }
}

static long int heightNode(NodeIndiceBST *node) {
    if (node == NULL) {
        return 0;
    }
    long int hEsq = heightNode(node->esq);
    long int hDir = heightNode(node->dir);
    return (hEsq > hDir ? hEsq : hDir) + 1;
}

static void inOrder(NodeIndiceBST *node, FILE *output) {
    if (node != NULL) {
        inOrder(node->esq, output);
        fprintf(output, "<%ld, %ld>\n", node->codigo, node->nreg);
        inOrder(node->dir, output);
    }
}

// Funções públicas

IndiceBST *createIndiceBST(void) {
    IndiceBST *bst = (IndiceBST *)malloc(sizeof(IndiceBST));
    if (bst == NULL) {
        fprintf(stderr, "Erro: falha ao alocar IndiceBST\n");
        exit(EXIT_FAILURE);
    }
    bst->raiz = NULL;
    bst->tamanho = 0;
    return bst;
}

void destroyIndiceBST(IndiceBST *self) {
    if (self != NULL) {
        destroyNode(self->raiz);
        free(self);
    }
}

void insertIndiceBST(IndiceBST *self, long int codigo, long int nreg) {
    if (self == NULL) return;
    int inserido = 0;
    self->raiz = insertNode(self->raiz, codigo, nreg, &inserido);
    if (inserido) {
        self->tamanho++;
    }
}

long int searchIndiceBST(IndiceBST *self, long int codigo) {
    if (self == NULL) return -1;
    NodeIndiceBST *result = searchNode(self->raiz, codigo);
    if (result != NULL) {
        return result->nreg;
    }
    return -1;
}

long int sizeIndiceBST(IndiceBST *self) {
    if (self == NULL) return 0;
    return self->tamanho;
}

long int heightIndiceBST(IndiceBST *self) {
    if (self == NULL) return 0;
    return heightNode(self->raiz);
}

void printParesIndiceBST(IndiceBST *self, FILE *output) {
    if (self == NULL) return;
    if (output == NULL) {
        output = stdout;
    }
    inOrder(self->raiz, output);
}
