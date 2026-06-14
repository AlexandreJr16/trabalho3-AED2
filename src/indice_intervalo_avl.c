#include "indice_intervalo_avl.h"
#include <stdlib.h>
#include <stdio.h>

// 2.1 NodeIntervaloAVL
typedef struct NodeIntervaloAVL {
    double preco;
    long int nreg;
    int altura;
    struct NodeIntervaloAVL *left;
    struct NodeIntervaloAVL *right;
} NodeIntervaloAVL;

struct IndiceIntervaloAVL {
    NodeIntervaloAVL *root;
    long int size;
};

// 2.2 Funções de Apoio (Altura, Balanço, Rotações)
static int getHeight(NodeIntervaloAVL *node) {
    if (node == NULL) return 0;
    return node->altura;
}

static int getBalance(NodeIntervaloAVL *node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static void updateHeight(NodeIntervaloAVL *node) {
    if (node != NULL) {
        node->altura = 1 + max(getHeight(node->left), getHeight(node->right));
    }
}

// Rotação à Esquerda (Left Rotation - para nós desbalanceados pendendo à direita)
static NodeIntervaloAVL *rotateLeft(NodeIntervaloAVL *x) {
    NodeIntervaloAVL *y = x->right;
    NodeIntervaloAVL *T2 = y->left;

    // Executa a rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    updateHeight(x);
    updateHeight(y);

    return y;
}

// Rotação à Direita (Right Rotation - para nós desbalanceados pendendo à esquerda)
static NodeIntervaloAVL *rotateRight(NodeIntervaloAVL *y) {
    NodeIntervaloAVL *x = y->left;
    NodeIntervaloAVL *T2 = x->right;

    // Executa a rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas
    updateHeight(y);
    updateHeight(x);

    return x;
}

// Rotações duplas não precisam de funções separadas; basta compor.

// 2.3 Recursão de Inserção com propagação de empates para a direita
static NodeIntervaloAVL *insertNode(NodeIntervaloAVL *node, double preco, long int nreg, long int *sizeOut) {
    // 1. Inserção normal de BST
    if (node == NULL) {
        NodeIntervaloAVL *newNode = (NodeIntervaloAVL *)malloc(sizeof(NodeIntervaloAVL));
        if (newNode != NULL) {
            newNode->preco = preco;
            newNode->nreg = nreg;
            newNode->altura = 1;
            newNode->left = NULL;
            newNode->right = NULL;
            (*sizeOut)++;
        }
        return newNode;
    }

    if (preco < node->preco) {
        node->left = insertNode(node->left, preco, nreg, sizeOut);
    } else {
        // preco >= node->preco: empates ou maiores vão para a direita (conforme enunciado)
        node->right = insertNode(node->right, preco, nreg, sizeOut);
    }

    // 2. Atualizar altura
    updateHeight(node);

    // 3. Obter fator de balanceamento
    int balance = getBalance(node);

    // 4. Balancear a árvore
    // Left Left Case
    if (balance > 1 && preco < node->left->preco) {
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && preco >= node->right->preco) {
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && preco >= node->left->preco) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && preco < node->right->preco) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Retorna o nó (inalterado ou a nova raiz da subárvore)
    return node;
}

IndiceIntervaloAVL *createIndiceIntervaloAVL(void) {
    IndiceIntervaloAVL *avl = (IndiceIntervaloAVL *)malloc(sizeof(IndiceIntervaloAVL));
    if (avl != NULL) {
        avl->root = NULL;
        avl->size = 0;
    }
    return avl;
}

static void freeNode(NodeIntervaloAVL *node) {
    if (node != NULL) {
        freeNode(node->left);
        freeNode(node->right);
        free(node);
    }
}

void destroyIndiceIntervaloAVL(IndiceIntervaloAVL *self) {
    if (self != NULL) {
        freeNode(self->root);
        free(self);
    }
}

void insertIndiceIntervaloAVL(IndiceIntervaloAVL *self, double preco, long int nreg) {
    if (self != NULL) {
        self->root = insertNode(self->root, preco, nreg, &(self->size));
    }
}

long int sizeIndiceIntervaloAVL(IndiceIntervaloAVL *self) {
    return (self != NULL) ? self->size : 0;
}

long int heightIndiceIntervaloAVL(IndiceIntervaloAVL *self) {
    return (self != NULL) ? getHeight(self->root) : 0;
}

// 2.4 Implementação da busca com poda recursiva
static void searchNode(NodeIntervaloAVL *node, double preco, Operador op, ListaLong *resultado) {
    if (node == NULL) return;

    int atende = 0;
    int explorarLeft = 1;
    int explorarRight = 1;

    switch (op) {
        case MAIOR:
            if (node->preco > preco) atende = 1;
            // Poda esquerda: se node->preco <= preco, tudo à esquerda é <= preco.
            if (node->preco <= preco) explorarLeft = 0;
            break;
        case MAIOR_IGUAL:
            if (node->preco >= preco) atende = 1;
            // Poda esquerda: se node->preco < preco, tudo à esquerda é < preco.
            if (node->preco < preco) explorarLeft = 0;
            break;
        case MENOR:
            if (node->preco < preco) atende = 1;
            // Poda direita: se node->preco >= preco, tudo à direita é >= preco.
            if (node->preco >= preco) explorarRight = 0;
            break;
        case MENOR_IGUAL:
            if (node->preco <= preco) atende = 1;
            // Poda direita: se node->preco > preco, tudo à direita é > preco.
            if (node->preco > preco) explorarRight = 0;
            break;
    }

    // Navega in-order (apenas nas subárvores não podadas)
    if (explorarLeft) {
        searchNode(node->left, preco, op, resultado);
    }
    
    if (atende) {
        addListaLong(resultado, node->nreg);
    }
    
    if (explorarRight) {
        searchNode(node->right, preco, op, resultado);
    }
}

void buscaIntervaloAVL(IndiceIntervaloAVL *self, double preco, Operador op, ListaLong *resultado) {
    if (self == NULL || resultado == NULL) return;
    searchNode(self->root, preco, op, resultado);
}
