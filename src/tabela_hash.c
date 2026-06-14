#include "tabela_hash.h"
#include <stdlib.h>

typedef struct NodeHash {
    long int codigo;
    long int nreg;
    struct NodeHash *proximo;
} NodeHash;

struct TabelaHash {
    NodeHash **buckets;
    long int tamanho;
    long int colisoes;
};

TabelaHash *createTabelaHash(long int tamanho) {
    TabelaHash *hash = (TabelaHash *)malloc(sizeof(TabelaHash));
    if (hash == NULL) {
        fprintf(stderr, "Erro: falha ao alocar TabelaHash\n");
        exit(EXIT_FAILURE);
    }
    hash->tamanho = tamanho;
    hash->colisoes = 0;
    
    hash->buckets = (NodeHash **)calloc(tamanho, sizeof(NodeHash *));
    if (hash->buckets == NULL) {
        fprintf(stderr, "Erro: falha ao alocar buckets da TabelaHash\n");
        free(hash);
        exit(EXIT_FAILURE);
    }
    
    return hash;
}

void destroyTabelaHash(TabelaHash *self) {
    if (self != NULL) {
        for (long int i = 0; i < self->tamanho; i++) {
            NodeHash *current = self->buckets[i];
            while (current != NULL) {
                NodeHash *temp = current;
                current = current->proximo;
                free(temp);
            }
        }
        free(self->buckets);
        free(self);
    }
}

void insertTabelaHash(TabelaHash *self, long int codigo, long int nreg) {
    if (self == NULL || self->tamanho == 0) return;
    
    long int indice = codigo % self->tamanho;
    
    // Verifica colisao (se ja existe algo no bucket antes da inserção)
    if (self->buckets[indice] != NULL) {
        self->colisoes++;
    }
    
    NodeHash *novoNode = (NodeHash *)malloc(sizeof(NodeHash));
    if (novoNode == NULL) {
        fprintf(stderr, "Erro: falha ao alocar NodeHash\n");
        exit(EXIT_FAILURE);
    }
    
    novoNode->codigo = codigo;
    novoNode->nreg = nreg;
    
    // Inserção no início da lista (encadeamento)
    novoNode->proximo = self->buckets[indice];
    self->buckets[indice] = novoNode;
}

long int searchTabelaHash(TabelaHash *self, long int codigo) {
    if (self == NULL || self->tamanho == 0) return -1;
    
    long int indice = codigo % self->tamanho;
    NodeHash *current = self->buckets[indice];
    
    while (current != NULL) {
        if (current->codigo == codigo) {
            return current->nreg;
        }
        current = current->proximo;
    }
    return -1;
}

long int getColisoesTabelaHash(TabelaHash *self) {
    if (self == NULL) return 0;
    return self->colisoes;
}

void printParesTabelaHash(TabelaHash *self, FILE *output) {
    if (self == NULL) return;
    if (output == NULL) {
        output = stdout;
    }
    
    for (long int i = 0; i < self->tamanho; i++) {
        NodeHash *current = self->buckets[i];
        if (current != NULL) {
            fprintf(output, "Bucket %ld: ", i);
            while (current != NULL) {
                fprintf(output, "<%ld, %ld> ", current->codigo, current->nreg);
                current = current->proximo;
            }
            fprintf(output, "\n");
        }
    }
}
