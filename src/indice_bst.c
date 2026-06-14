#include "indice_bst.h"
#include <stdlib.h>

// ======== Estruturas Internas ========

typedef struct NodeIndiceBST {
    long int codigo;
    long int nreg;
    struct NodeIndiceBST *left;
    struct NodeIndiceBST *right;
} NodeIndiceBST;

struct IndiceBST {
    NodeIndiceBST *root;
    long int tamanho;
};

// ======== Funções Auxiliares (Estáticas) ========

static NodeIndiceBST *createNodeIndiceBST(long int codigo, long int nreg) {
    NodeIndiceBST *node = (NodeIndiceBST *)malloc(sizeof(NodeIndiceBST));
    if (node == NULL) {
        fprintf(stderr, "Erro: falha ao alocar NodeIndiceBST\n");
        exit(EXIT_FAILURE);
    }
    node->codigo = codigo;
    node->nreg = nreg;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void destroyNode(NodeIndiceBST *node) {
    if (node != NULL) {
        destroyNode(node->left);
        destroyNode(node->right);
        free(node);
    }
}

static NodeIndiceBST *insertNode(NodeIndiceBST *node, long int codigo, long int nreg, int *inserido) {
    if (node == NULL) {
        *inserido = 1;
        return createNodeIndiceBST(codigo, nreg);
    }

    if (codigo < node->codigo) {
        node->left = insertNode(node->left, codigo, nreg, inserido);
    } else if (codigo > node->codigo) {
        node->right = insertNode(node->right, codigo, nreg, inserido);
    }
    // Se codigo == node->codigo, já existe, ignoramos.
    
    return node;
}

static NodeIndiceBST *searchNode(NodeIndiceBST *node, long int codigo) {
    if (node == NULL) {
        return NULL;
    }
    if (codigo < node->codigo) {
        return searchNode(node->left, codigo);
    } else if (codigo > node->codigo) {
        return searchNode(node->right, codigo);
    } else {
        return node;
    }
}

static long int heightNode(NodeIndiceBST *node) {
    if (node == NULL) {
        return 0;
    }
    long int hLeft = heightNode(node->left);
    long int hRight = heightNode(node->right);
    return (hLeft > hRight ? hLeft : hRight) + 1;
}

static void inOrder(NodeIndiceBST *node, FILE *output) {
    if (node != NULL) {
        inOrder(node->left, output);
        fprintf(output, "<%ld, %ld>\n", node->codigo, node->nreg);
        inOrder(node->right, output);
    }
}

// ======== Funções Públicas ========

IndiceBST *createIndiceBST(void) {
    IndiceBST *bst = (IndiceBST *)malloc(sizeof(IndiceBST));
    if (bst == NULL) {
        fprintf(stderr, "Erro: falha ao alocar IndiceBST\n");
        exit(EXIT_FAILURE);
    }
    bst->root = NULL;
    bst->tamanho = 0;
    return bst;
}

void destroyIndiceBST(IndiceBST *self) {
    if (self != NULL) {
        destroyNode(self->root);
        free(self);
    }
}

void insertIndiceBST(IndiceBST *self, long int codigo, long int nreg) {
    if (self == NULL) return;
    int inserido = 0;
    self->root = insertNode(self->root, codigo, nreg, &inserido);
    if (inserido) {
        self->tamanho++;
    }
}

long int searchIndiceBST(IndiceBST *self, long int codigo) {
    if (self == NULL) return -1;
    NodeIndiceBST *result = searchNode(self->root, codigo);
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
    return heightNode(self->root);
}

void printParesIndiceBST(IndiceBST *self, FILE *output) {
    if (self == NULL) return;
    if (output == NULL) {
        output = stdout;
    }
    inOrder(self->root, output);
}
