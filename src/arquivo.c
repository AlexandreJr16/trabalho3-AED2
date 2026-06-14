#include "arquivo.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

struct ArquivoProdutos {
    char *caminho;
    long int quantidade;
    FILE *fp;
};

// Isso usa apenas pra gerar os produtos
static const char *CATEGORIAS[] = {
    "Eletronicos", "Alimentos", "Vestuario",
    "Papelaria", "Esportes", "Brinquedos"
};
static const int NUM_CATEGORIAS = 6;

// Construtor / Destrutor

ArquivoProdutos *createArquivoProdutos(const char *caminho) {
    ArquivoProdutos *arq = (ArquivoProdutos *)malloc(sizeof(ArquivoProdutos));
    if (arq == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para ArquivoProdutos\n");
        exit(EXIT_FAILURE);
    }

    arq->caminho = (char *)malloc(strlen(caminho) + 1);
    if (arq->caminho == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para caminho\n");
        free(arq);
        exit(EXIT_FAILURE);
    }
    strcpy(arq->caminho, caminho);
    arq->quantidade = 0;
    arq->fp = NULL;

    return arq;
}

void destroyArquivoProdutos(ArquivoProdutos *self) {
    if (self != NULL) {
        if (self->fp != NULL) {
            fclose(self->fp);
        }
        free(self->caminho);
        free(self);
    }
}

// Operaçoes publicas

void gerarArquivoProdutos(ArquivoProdutos *self, long int quantidade, long int range_codigo) {
    // Sortear codigos únicos
    long int *codigos = (long int *)malloc(quantidade * sizeof(long int));
    if (codigos == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para codigos\n");
        exit(EXIT_FAILURE);
    }
    gerarValoresUnicos(codigos, quantidade, range_codigo);

    // Abrir arquivo (escreve em bninario)
    FILE *fp = fopen(self->caminho, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir arquivo '%s' para escrita\n", self->caminho);
        free(codigos);
        exit(EXIT_FAILURE);
    }

    // Essa parte vai gerar e gravar o prod
    for (long int i = 0; i < quantidade; i++) {
        char nome_temp[50];
        snprintf(nome_temp, sizeof(nome_temp), "Produto_%ld", codigos[i]);

        Produto p;
        p.codigo = codigos[i];
        strncpy(p.nome, nome_temp, sizeof(p.nome) - 1);
        p.nome[sizeof(p.nome) - 1] = '\0';
        strncpy(p.categoria, CATEGORIAS[genRandomNumber(0, NUM_CATEGORIAS - 1)],
                sizeof(p.categoria) - 1);
        p.categoria[sizeof(p.categoria) - 1] = '\0';
        p.preco = 1.00 + (double)(rand() % 99900) / 100.0;  
        p.quantidade_estoque = (int)genRandomNumber(1, 1000);
        p.codigo_fornecedor = genRandomNumber(1, 100);

        fwrite(&p, sizeof(Produto), 1, fp);
    }

    fclose(fp);
    free(codigos);
    self->quantidade = quantidade;

    if (self->fp != NULL) {
        fclose(self->fp);
        self->fp = NULL;
    }
}

static int abrirSeNecessario(ArquivoProdutos *self) {
    if (self->fp == NULL) {
        self->fp = fopen(self->caminho, "rb");
        if (self->fp == NULL) return 0;
    }
    return 1;
}

int lerProdutoArquivo(ArquivoProdutos *self, long int nreg, Produto *out) {
    if (nreg < 0 || nreg >= self->quantidade) {
        return 0;
    }
    if (!abrirSeNecessario(self)) {
        return 0;
    }

    fseek(self->fp, nreg * (long int)sizeof(Produto), SEEK_SET);
    size_t lidos = fread(out, sizeof(Produto), 1, self->fp);

    return (lidos == 1) ? 1 : 0;
}

long int getQuantidadeArquivo(ArquivoProdutos *self) {
    return self->quantidade;
}

long int buscaSequencialCodigo(ArquivoProdutos *self, long int codigo, Produto *out) {
    if (self == NULL || self->caminho == NULL) return -1;
    if (!abrirSeNecessario(self)) return -1;

    rewind(self->fp);
    Produto p;
    long int nreg = 0;
    
    while (fread(&p, sizeof(Produto), 1, self->fp) == 1) {
        if (p.codigo == codigo) {
            if (out != NULL) {
                *out = p;
            }
            return nreg;
        }
        nreg++;
    }
    
    return -1;
}

void buscaSequencialIntervalo(ArquivoProdutos *self, double preco, Operador op, ListaProdutos *resultado) {
    if (self == NULL || self->caminho == NULL || resultado == NULL) return;
    if (!abrirSeNecessario(self)) return;

    rewind(self->fp);
    Produto p;
    while (fread(&p, sizeof(Produto), 1, self->fp) == 1) {
        int atende = 0;
        switch (op) {
            case MAIOR:       if (p.preco > preco) atende = 1; break;
            case MAIOR_IGUAL: if (p.preco >= preco) atende = 1; break;
            case MENOR:       if (p.preco < preco) atende = 1; break;
            case MENOR_IGUAL: if (p.preco <= preco) atende = 1; break;
        }
        if (atende) {
            addListaProdutos(resultado, p);
        }
    }
}

