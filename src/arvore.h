/*
 * arvore.h
 * ========
 * Interface pública da Árvore Binária de Busca (ABB / BST).
 *
 * A ABB permite busca, inserção e travessia em O(log n) no caso médio
 * (O(n) no pior caso, quando a árvore fica degenerada em lista).
 *
 * Chave de busca: campo `id` do Atendimento (inteiro único por registro).
 *
 * Propriedade da ABB:
 *   - Para cada nó N:
 *     * Todos os nós na subárvore ESQUERDA têm id < N.dado.id
 *     * Todos os nós na subárvore DIREITA  têm id > N.dado.id
 *
 * Cada nó da árvore armazena uma CÓPIA do Atendimento.
 */

#ifndef ARVORE_H
#define ARVORE_H

#include "atendimento.h"

/* ------------------------------------------------------------------ */
/* Nó da Árvore Binária de Busca                                       */
/* ------------------------------------------------------------------ */
typedef struct NoArvore {
    Atendimento dado;           /* Cópia do atendimento armazenado   */
    struct NoArvore *esquerda;  /* Subárvore com IDs menores          */
    struct NoArvore *direita;   /* Subárvore com IDs maiores          */
} NoArvore;

/* ------------------------------------------------------------------ */
/* Estrutura da Árvore                                                  */
/* ------------------------------------------------------------------ */
typedef struct {
    NoArvore *raiz; /* Ponteiro para o nó raiz */
    int       tamanho;
} Arvore;

/* ------------------------------------------------------------------ */
/* Protótipos das funções públicas                                      */
/* ------------------------------------------------------------------ */

/* Inicializa a árvore com raiz nula */
void arvore_inicializar(Arvore *a);

/* Insere uma cópia do atendimento na posição correta segundo o id.
   Retorna 1 em sucesso, 0 em falha ou id duplicado. */
int arvore_inserir(Arvore *a, Atendimento at);

/* Busca um atendimento pelo id. Retorna ponteiro para o dado se encontrado,
   NULL caso contrário. */
const Atendimento *arvore_buscar(const Arvore *a, int id);

/* Exibe a árvore em percurso in-ordem (resulta em saída ordenada por id) */
void arvore_imprimir_inordem(const Arvore *a);

/* Libera todos os nós da árvore recursivamente */
void arvore_destruir(Arvore *a);

/* ==================================================================
 * ABB DE INTEIROS — usada nos experimentos de comparação de buscas
 *
 * Implementação separada para não misturar com o TAD Atendimento.
 * Todas as operações são ITERATIVAS para suportar até 1.000.000
 * de elementos sem risco de estouro de pilha de chamadas.
 * ================================================================== */

/* Nó da ABB de inteiros */
typedef struct NoInt {
    int valor;
    struct NoInt *esquerda;
    struct NoInt *direita;
} NoInt;

/* Estrutura da ABB de inteiros */
typedef struct {
    NoInt *raiz;
    int    tamanho;
} ArvoreInt;

/* Inicializa a árvore */
void arvoreInt_inicializar(ArvoreInt *a);

/* Insere um inteiro. Retorna 1 em sucesso, 0 se duplicado ou sem memória.
   Implementação ITERATIVA (suporta árvores degeneradas de 1M nós). */
int arvoreInt_inserir(ArvoreInt *a, int valor);

/* Busca com contagem de comparações (iterativa). Retorna 1 se encontrou. */
int buscarNaArvore(NoInt *raiz, int valor, int *comparacoes);

/* Altura da árvore usando BFS iterativo (sem risco de stack overflow). */
int alturaArvore(NoInt *raiz);

/* Libera todos os nós usando rotações (O(n), O(1) espaço). */
void arvoreInt_destruir(ArvoreInt *a);

#endif /* ARVORE_H */
