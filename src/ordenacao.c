/*
 * ordenacao.c
 * ===========
 * Implementação do Merge Sort para Lista Encadeada.
 *
 * O Merge Sort em lista encadeada funciona em três etapas:
 *
 *   1. DIVISÃO (merge_sort_dividir):
 *      Usa a técnica do "ponteiro lento e rápido" (Floyd's cycle detection)
 *      para encontrar o meio da lista e dividi-la em duas sublistas.
 *      O ponteiro rápido avança 2 nós por vez; o lento avança 1.
 *      Quando o rápido chega ao fim, o lento está no meio.
 *
 *   2. RECURSÃO:
 *      Cada metade é ordenada recursivamente até chegar a listas de tamanho 1
 *      (caso base: lista nula ou com um único nó, já está ordenada).
 *
 *   3. MESCLAGEM (merge_sorted):
 *      Percorre as duas sublistas simultaneamente, comparando nó a nó e
 *      encadeando em ordem os menores primeiro. Ao esgotar uma sublista,
 *      encadeia o restante da outra.
 *
 * Critério de comparação (função comparar):
 *   - Prioridade decrescente: nó com prioridade MAIOR vem primeiro.
 *   - Empate de prioridade: nó com tempo_estimado MENOR vem primeiro.
 *   - Retorna negativo se 'a' deve vir ANTES de 'b'.
 */

#include <stdio.h>
#include "ordenacao.h"

/* ------------------------------------------------------------------
 * comparar
 * Função de comparação entre dois Atendimentos.
 * Retorna valor negativo se 'a' deve preceder 'b' na lista ordenada.
 * ------------------------------------------------------------------ */
static int comparar(const Atendimento *a, const Atendimento *b) {
    /* Critério primário: prioridade decrescente */
    if (a->prioridade != b->prioridade) {
        return b->prioridade - a->prioridade; /* maior prioridade vem antes */
    }
    /* Critério secundário: tempo estimado crescente (menor tempo primeiro) */
    return a->tempo_estimado - b->tempo_estimado;
}

/* ------------------------------------------------------------------
 * merge_sorted
 * Mescla duas sublistas já ordenadas em uma única lista ordenada.
 * Retorna o ponteiro para o novo nó inicial (cabeça da lista mesclada).
 * ------------------------------------------------------------------ */
static NoLista *merge_sorted(NoLista *esq, NoLista *dir) {
    /* Caso base: se uma das listas é vazia, retorna a outra */
    if (esq == NULL) return dir;
    if (dir == NULL) return esq;

    NoLista *resultado;

    if (comparar(&esq->dado, &dir->dado) <= 0) {
        /* O nó da esquerda deve vir primeiro */
        resultado          = esq;
        resultado->proximo = merge_sorted(esq->proximo, dir);
    } else {
        /* O nó da direita deve vir primeiro */
        resultado          = dir;
        resultado->proximo = merge_sorted(esq, dir->proximo);
    }

    return resultado;
}

/* ------------------------------------------------------------------
 * merge_sort_dividir
 * Divide a lista encadeada ao meio usando a técnica do ponteiro rápido/lento.
 * Após a chamada, `cabeca` contém a primeira metade e `*segunda_metade`
 * contém a segunda metade.
 * ------------------------------------------------------------------ */
static void merge_sort_dividir(NoLista *cabeca, NoLista **segunda_metade) {
    NoLista *lento = cabeca;       /* Avança 1 nó por iteração */
    NoLista *rapido = cabeca->proximo; /* Avança 2 nós por iteração */

    /* Avança até o ponteiro rápido chegar ao fim */
    while (rapido != NULL) {
        rapido = rapido->proximo;
        if (rapido != NULL) {
            lento  = lento->proximo;
            rapido = rapido->proximo;
        }
    }

    /* `lento` agora aponta para o nó do meio — corta a lista aqui */
    *segunda_metade   = lento->proximo;
    lento->proximo    = NULL; /* Termina a primeira metade */
}

/* ------------------------------------------------------------------
 * merge_sort_rec
 * Função recursiva interna do Merge Sort.
 * Recebe e retorna o ponteiro para a cabeça da (sub)lista.
 * ------------------------------------------------------------------ */
static NoLista *merge_sort_rec(NoLista *cabeca) {
    /* Caso base: lista vazia ou com um único nó — já está ordenada */
    if (cabeca == NULL || cabeca->proximo == NULL) {
        return cabeca;
    }

    NoLista *segunda_metade;

    /* Divide a lista em duas metades */
    merge_sort_dividir(cabeca, &segunda_metade);

    /* Ordena recursivamente cada metade */
    cabeca         = merge_sort_rec(cabeca);
    segunda_metade = merge_sort_rec(segunda_metade);

    /* Mescla as duas metades ordenadas */
    return merge_sorted(cabeca, segunda_metade);
}

/* ------------------------------------------------------------------
 * ordenar_lista
 * Ponto de entrada público. Ordena a lista e recalcula o ponteiro
 * de cauda após a reorganização dos nós.
 * ------------------------------------------------------------------ */
void ordenar_lista(Lista *l) {
    if (l->tamanho < 2) {
        /* Lista vazia ou com 1 elemento: já está ordenada */
        return;
    }

    /* Executa o Merge Sort; a nova cabeça pode ser diferente da original */
    l->cabeca = merge_sort_rec(l->cabeca);

    /* Recalcula a cauda percorrendo até o último nó */
    NoLista *atual = l->cabeca;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    l->cauda = atual;

    printf("  [Ordenacao] Historico ordenado por prioridade (desc) e tempo estimado (asc).\n");
}
