/*
 * buscas.c
 * ========
 * Implementação dos algoritmos de busca sequencial e binária,
 * além dos utilitários de geração e ordenação de vetores.
 */

#include <stdio.h>
#include <stdlib.h>
#include "buscas.h"

/* ------------------------------------------------------------------
 * buscaSequencial
 * Itera do índice 0 ao (tamanho-1). Em cada posição incrementa o
 * contador de comparações ANTES de comparar (cada acesso ao elemento
 * conta como uma comparação).
 *
 * Complexidade: O(n) no pior caso (elemento ausente ou no fim).
 * ------------------------------------------------------------------ */
int buscaSequencial(int vetor[], int tamanho, int valor, int *comparacoes) {
    int i;
    for (i = 0; i < tamanho; i++) {
        (*comparacoes)++;
        if (vetor[i] == valor) {
            return i; /* Retorna o índice onde encontrou */
        }
    }
    return -1; /* Não encontrado */
}

/* ------------------------------------------------------------------
 * buscaBinaria
 * Mantém um intervalo [esq, dir]. A cada passo computa o índice do
 * meio e compara com `valor`:
 *   - Igual:  encontrou, retorna o índice.
 *   - Menor:  valor está à direita do meio → esq = meio + 1.
 *   - Maior:  valor está à esquerda do meio → dir = meio - 1.
 *
 * Conta DUAS comparações quando descarta metade (== e <), e UMA
 * quando encontra (apenas ==). Para manter a contagem conservadora
 * e compatível com a literatura, conta 1 comparação por iteração
 * (apenas a comparação com o elemento do meio).
 *
 * Complexidade: O(log n) — reduz o espaço de busca pela metade a cada passo.
 * ------------------------------------------------------------------ */
int buscaBinaria(int vetor[], int tamanho, int valor, int *comparacoes) {
    int esq  = 0;
    int dir  = tamanho - 1;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2; /* Evita overflow vs (esq+dir)/2 */

        (*comparacoes)++;
        if (vetor[meio] == valor) {
            return meio;
        }

        (*comparacoes)++;
        if (vetor[meio] < valor) {
            esq = meio + 1;  /* Descarta metade esquerda */
        } else {
            dir = meio - 1;  /* Descarta metade direita */
        }
    }
    return -1; /* Não encontrado */
}

/* ------------------------------------------------------------------
 * comparar_int (interno)
 * Função de comparação exigida pelo qsort para inteiros.
 * Subtração direta funciona quando não há risco de overflow.
 * Como os valores são 1..N com N <= 1.000.000, está seguro.
 * ------------------------------------------------------------------ */
static int comparar_int(const void *a, const void *b) {
    return (*(const int *)a) - (*(const int *)b);
}

/* ------------------------------------------------------------------
 * gerarVetorOrdenado
 * Aloca vetor e preenche com 1, 2, ..., tamanho.
 * ------------------------------------------------------------------ */
int *gerarVetorOrdenado(int tamanho) {
    int *v = (int *)malloc((size_t)tamanho * sizeof(int));
    if (!v) {
        fprintf(stderr, "[BUSCAS] Erro: falha ao alocar vetor de %d elementos.\n",
                tamanho);
        return NULL;
    }
    int i;
    for (i = 0; i < tamanho; i++) {
        v[i] = i + 1;
    }
    return v;
}

/* ------------------------------------------------------------------
 * gerarVetorEmbaralhado
 * Gera vetor 1..n e aplica o algoritmo Fisher-Yates (Knuth shuffle):
 *
 *   Para i de n-1 até 1:
 *     j = número aleatório em [0, i]
 *     troca v[i] com v[j]
 *
 * Complexidade: O(n) — cada elemento é trocado no máximo uma vez.
 * Semente fixa (42) garante reprodutibilidade dos experimentos.
 * ------------------------------------------------------------------ */
int *gerarVetorEmbaralhado(int tamanho) {
    int *v = gerarVetorOrdenado(tamanho);
    if (!v) return NULL;

    srand(42); /* Semente fixa para reprodutibilidade */

    int i;
    for (i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        /* Troca v[i] com v[j] */
        int tmp = v[i];
        v[i]    = v[j];
        v[j]    = tmp;
    }
    return v;
}

/* ------------------------------------------------------------------
 * ordenarVetor
 * Delega para qsort da stdlib.h com o comparador de inteiros.
 * ------------------------------------------------------------------ */
void ordenarVetor(int vetor[], int tamanho) {
    qsort(vetor, (size_t)tamanho, sizeof(int), comparar_int);
}
