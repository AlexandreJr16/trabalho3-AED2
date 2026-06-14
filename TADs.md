# Definição dos Tipos Abstratos de Dados (TAD) — Trabalho 3

## Convenções gerais

- `li` = `long int` (igual ao T2)
- `nreg` = número do registro dentro do arquivo (posição 0-indexada), usado com
  `fseek(arquivo, nreg * sizeof(Produto), SEEK_SET)` para acesso direto
- Mesmo padrão de organização do T2: TADs opacas (`typedef struct X X;` no `.h`,
  struct completa só no `.c`), construtores `createX`, destrutores `destroyX`
- Toda função que imprime recebe `FILE *output` (se `NULL`, usa `stdout`) —
  mesmo padrão do `showResults` em `utils.c` do T2
- Convenção de medição na Q1: cada tempo mede o **pipeline completo**
  (busca no índice + `lerProdutoArquivo` para trazer o registro do disco),
  refletindo o custo real de uma consulta end-to-end
- Tipo auxiliar `Operador` (definido em `include/operador.h`), usado nas
  consultas por intervalo (tipos 4 e 5):

```c
typedef enum {
  MAIOR,        // preco > valor
  MAIOR_IGUAL,  // preco >= valor
  MENOR,        // preco < valor
  MENOR_IGUAL   // preco <= valor
} Operador;
```

---

## Produto

Registro de dados gravado no arquivo binário. Não é uma TAD com operações,
é a struct que define o layout de cada registro do arquivo.

### Atributos do Produto

- codigo: li — campo chave, valores únicos
- nome: char[50]
- categoria: char[20]
- preco: double — atributo usado nas consultas por intervalo (`>`, `>=`, `<`, `<=`)
- quantidade_estoque: int
- codigo_fornecedor: li

`sizeof(Produto) ≈ 104 bytes` (x86-64, alinhamento padrão do gcc), próximo dos
"~100 bytes" sugeridos no enunciado.

---

## ArquivoProdutos

TAD que encapsula o arquivo binário de registros e as buscas que acessam o
disco diretamente.

### Atributos do ArquivoProdutos

- caminho: char* — caminho do arquivo em disco
- quantidade: li — número de registros no arquivo

### Construtores / Destrutores do ArquivoProdutos

- createArquivoProdutos: ArquivoProdutos* (const char *caminho)
- destroyArquivoProdutos: void (ArquivoProdutos *self)

### Operações Públicas do ArquivoProdutos

- gerarArquivoProdutos: void (ArquivoProdutos *self, li quantidade, li range_codigo)
  — gera `quantidade` produtos com `codigo` únicos sorteados em `[1, range_codigo]`
  e demais campos aleatórios; grava no arquivo binário (sobrescrevendo) e
  atualiza `quantidade`
- lerProdutoArquivo: int (ArquivoProdutos *self, li nreg, Produto *out)
  — `fseek` + `fread` de um registro; retorna 1 se `nreg` é válido, 0 caso contrário
- getQuantidadeArquivo: li (ArquivoProdutos *self)
- buscaSequencialCodigo: li (ArquivoProdutos *self, li codigo, Produto *out)
  — busca tipo 3: varre o arquivo do início ao fim comparando `codigo`;
  se encontrar, copia o registro para `out` e retorna `nreg`; senão retorna -1
- buscaSequencialIntervalo: void (ArquivoProdutos *self, Operador operador, double valor, ListaProdutos *resultado)
  — busca tipo 5: varre o arquivo do início ao fim; para cada registro cujo
  `preco` satisfaça `preco operador valor`, adiciona uma cópia em `resultado`

---

## ListaProdutos

Lista dinâmica de `Produto`, usada para acumular o resultado das consultas
por intervalo (tipos 4 e 5).

### Atributos da ListaProdutos

- dados: Produto* — vetor alocado dinamicamente
- tamanho: li — quantidade de elementos armazenados
- capacidade: li — capacidade alocada atualmente

### Construtores / Destrutores da ListaProdutos

- createListaProdutos: ListaProdutos* ()
- destroyListaProdutos: void (ListaProdutos *self)

### Operações Públicas da ListaProdutos

- addListaProdutos: void (ListaProdutos *self, Produto p)
  — adiciona ao final, dobrando a capacidade quando necessário
- getTamanhoListaProdutos: li (ListaProdutos *self)
- getProdutoListaProdutos: Produto (ListaProdutos *self, li indice)
- printListaProdutos: void (ListaProdutos *self, FILE *output)
  — imprime todos os campos de cada produto da lista, um por linha

---

## ListaLong

Lista dinâmica de `li`, usada para acumular `nreg` durante as buscas por
índice (BST/Hash/AVL retornam números de registro, não produtos).

### Atributos da ListaLong

- dados: li* — vetor alocado dinamicamente
- tamanho: li
- capacidade: li

### Construtores / Destrutores da ListaLong

- createListaLong: ListaLong* ()
- destroyListaLong: void (ListaLong *self)

### Operações Públicas da ListaLong

- addListaLong: void (ListaLong *self, li valor)
- getTamanhoListaLong: li (ListaLong *self)
- getElementoListaLong: li (ListaLong *self, li indice)

---

## IndiceBST (índice da chave — busca tipo 1)

Árvore binária de pesquisa **não-balanceada**, cujos nós guardam pares
`<codigo, nreg>`. Mesmo padrão de `bst.c` do T2, adaptado para pares
(chave única `codigo`).

### Atributos do IndiceBST

- raiz: ponteiro para NodeIndiceBST
- tamanho: li

### Atributos do NodeIndiceBST (interno)

- codigo: li
- nreg: li
- esq: ponteiro para NodeIndiceBST
- dir: ponteiro para NodeIndiceBST

### Construtores / Destrutores do IndiceBST

- createIndiceBST: IndiceBST* ()
- destroyIndiceBST: void (IndiceBST *self)

### Operações Públicas do IndiceBST

- insertIndiceBST: void (IndiceBST *self, li codigo, li nreg)
- searchIndiceBST: li (IndiceBST *self, li codigo)
  — retorna `nreg` correspondente, ou -1 se `codigo` não existe no índice
- sizeIndiceBST: li (IndiceBST *self)
- heightIndiceBST: int (IndiceBST *self)
- printParesIndiceBST: void (IndiceBST *self, FILE *output)
  — percurso in-order; imprime cada par `<codigo, nreg>` (saída ordenada por codigo)

### Operações Internas do IndiceBST

- createNodeIndiceBST: NodeIndiceBST* (li codigo, li nreg)
- destroyNode: void (NodeIndiceBST *node) — libera recursivamente
- insertNode: NodeIndiceBST* (NodeIndiceBST *node, li codigo, li nreg, int *inserido)
- searchNode: li (NodeIndiceBST *node, li codigo)
- heightNode: int (NodeIndiceBST *node)
- inOrder: void (NodeIndiceBST *node, FILE *output)

---

## TabelaHash (índice da chave — busca tipo 2)

Tabela hash com **encadeamento** (lista ligada por bucket), guardando pares
`<codigo, nreg>`.

### Atributos da TabelaHash

- buckets: NodeHash** — vetor de listas encadeadas, tamanho `tamanho_tabela`
- tamanho_tabela: li
- quantidade: li — total de pares inseridos
- colisoes: li — contador acumulado de colisões

### Atributos do NodeHash (interno)

- codigo: li
- nreg: li
- proximo: ponteiro para NodeHash

### Construtores / Destrutores da TabelaHash

- createTabelaHash: TabelaHash* (li tamanho_tabela)
  — aloca `buckets` com `tamanho_tabela` posições, todas inicializadas com `NULL`
- destroyTabelaHash: void (TabelaHash *self)

### Operações Públicas da TabelaHash

- insertTabelaHash: void (TabelaHash *self, li codigo, li nreg)
  — calcula o bucket via `hashFunction`; **se o bucket já tiver algum nó antes
  da inserção, incrementa `colisoes`**; insere o novo nó no início da lista do bucket
- searchTabelaHash: li (TabelaHash *self, li codigo)
  — percorre a lista do bucket correspondente; retorna `nreg` ou -1
- getColisoesTabelaHash: li (TabelaHash *self)
- printParesTabelaHash: void (TabelaHash *self, FILE *output)
  — percorre todos os buckets, em ordem, e imprime cada par `<codigo, nreg>`
  (mesma informação do `printParesIndiceBST`, porém **fora de ordem por codigo** —
  serve para o teste da Dica 2: o **conjunto** de pares deve ser igual)

### Operações Internas da TabelaHash

- hashFunction: li (li codigo, li tamanho_tabela)
  — `codigo % tamanho_tabela`, garantindo resultado não-negativo
- createNodeHash: NodeHash* (li codigo, li nreg)
- destroyNodeHash: void (NodeHash *node)

---

## IndiceIntervaloAVL (índice do atributo de intervalo — busca tipo 4)

Árvore **AVL balanceada**, cujos nós guardam pares `<preco, nreg>`.
**Permite `preco` repetido** (duplicatas seguem sempre para a direita).
Mesmo padrão de rotações de `avl.c` do T2.

### Atributos do IndiceIntervaloAVL

- raiz: ponteiro para NodeIntervaloAVL
- tamanho: li

### Atributos do NodeIntervaloAVL (interno)

- preco: double
- nreg: li
- altura: int
- esq: ponteiro para NodeIntervaloAVL
- dir: ponteiro para NodeIntervaloAVL

### Construtores / Destrutores do IndiceIntervaloAVL

- createIndiceIntervaloAVL: IndiceIntervaloAVL* ()
- destroyIndiceIntervaloAVL: void (IndiceIntervaloAVL *self)

### Operações Públicas do IndiceIntervaloAVL

- insertIndiceIntervaloAVL: void (IndiceIntervaloAVL *self, double preco, li nreg)
  — se `preco` igual ao do nó atual, desce sempre para a direita (permite duplicatas)
- buscaIntervaloAVL: void (IndiceIntervaloAVL *self, Operador operador, double valor, ListaLong *resultado)
  — percorre a árvore **podando** subárvores que não podem satisfazer
  `preco operador valor` (ex: para `preco > valor`, ignora toda a subárvore
  esquerda de um nó cujo `preco <= valor`); adiciona o `nreg` de cada nó que
  satisfaz em `resultado`
- sizeIndiceIntervaloAVL: li (IndiceIntervaloAVL *self)
- heightIndiceIntervaloAVL: int (IndiceIntervaloAVL *self)
- printParesIndiceIntervaloAVL: void (IndiceIntervaloAVL *self, FILE *output)
  — percurso in-order, imprime `<preco, nreg>`

### Operações Internas do IndiceIntervaloAVL

- createNodeIntervaloAVL, freeNode, insertNode, heightNode, fatorBalanceamento,
  maxHeight, updateHeight, rotateLeft, rotateRight
  — mesmo padrão de nomes do `avl.c` do T2 (`heightNode`, `fatorBalanceamento`)
- buscaIntervalo: void (NodeIntervaloAVL *node, Operador operador, double valor, ListaLong *resultado)
  — versão recursiva interna com a lógica de poda
- inOrder: void (NodeIntervaloAVL *node, FILE *output)

---

## Utils (extensão do utils.c do T2)

### Operações reaproveitadas do T2 (copiar sem alteração)

- genRandomNumber: li (li min, li max)
- getMedia: double (double *tempos, li quantidade)
- getDesvioPadrao: double (double *tempos, li quantidade)
- showResults: void (double *tempos, li quantidade, FILE *output)

### Operações novas

- getTempoAtual: double ()
  — usa `clock_gettime(CLOCK_MONOTONIC, ...)`, retorna o tempo atual em
  segundos como `double` (resolução de nanossegundos, necessária porque
  cada busca roda **uma única vez** por valor, não em loop como no T2)
- gerarValoresUnicos: void (li *valores, li quantidade, li range)
  — sorteia `quantidade` valores **distintos** em `[1, range]`; usado tanto
  para os `codigo` dos produtos no arquivo quanto para os 30 valores de
  teste das buscas pela chave
