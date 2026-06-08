/*
 * hash.c
 * ======
 * Implementação da Tabela Hash com Encadeamento por Colisão.
 *
 * Função de hash djb2 (Daniel J. Bernstein):
 *   unsigned long hash = 5381;
 *   for cada caractere c: hash = ((hash << 5) + hash) + c;  // hash * 33 + c
 *   return hash % HASH_TAMANHO;
 *
 *   O valor inicial 5381 e o multiplicador 33 (2^5 + 1) foram escolhidos
 *   empiricamente por produzirem boa distribuição e poucas colisões.
 *
 * Inserção:
 *   Calcula índice via djb2, cria novo nó e insere no INÍCIO da lista do bucket
 *   (inserção no início é O(1) e evita percorrer a cadeia).
 *
 * Busca:
 *   Calcula índice da categoria procurada, percorre a lista do bucket
 *   comparando o campo `categoria` de cada nó com a string buscada.
 *
 * Destruição:
 *   Percorre todos os HASH_TAMANHO buckets e libera cada nó da cadeia.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* ------------------------------------------------------------------
 * hash_calcular (interno)
 * Algoritmo djb2 — mapeia uma string para um índice [0, HASH_TAMANHO).
 * ------------------------------------------------------------------ */
static unsigned int hash_calcular(const char *chave) {
    unsigned long h = 5381;
    int c;

    while ((c = (unsigned char)*chave++) != '\0') {
        h = ((h << 5) + h) + (unsigned long)c; /* h * 33 + c */
    }

    return (unsigned int)(h % HASH_TAMANHO);
}

/* ------------------------------------------------------------------
 * hash_inicializar
 * Zera todos os buckets da tabela.
 * ------------------------------------------------------------------ */
void hash_inicializar(TabelaHash *h) {
    int i;
    for (i = 0; i < HASH_TAMANHO; i++) {
        h->tabela[i] = NULL;
    }
    h->total = 0;
}

/* ------------------------------------------------------------------
 * hash_inserir
 * Calcula o índice para a categoria do atendimento e insere um novo nó
 * no início da lista desse bucket.
 * ------------------------------------------------------------------ */
int hash_inserir(TabelaHash *h, Atendimento a) {
    /* Calcula o índice usando a categoria como chave */
    unsigned int idx = hash_calcular(a.categoria);

    NoHash *novo = (NoHash *)malloc(sizeof(NoHash));
    if (!novo) {
        fprintf(stderr, "[HASH] Erro: falha ao alocar memoria.\n");
        return 0;
    }

    novo->dado = a;

    /* Insere no início da lista do bucket (O(1)) */
    novo->proximo  = h->tabela[idx];
    h->tabela[idx] = novo;

    h->total++;
    return 1;
}

/* ------------------------------------------------------------------
 * hash_buscar_por_categoria
 * Calcula o índice da categoria e percorre a lista do bucket,
 * imprimindo todos os atendimentos com essa categoria exata.
 * Retorna a quantidade de resultados encontrados.
 * ------------------------------------------------------------------ */
int hash_buscar_por_categoria(const TabelaHash *h, const char *categoria) {
    unsigned int idx = hash_calcular(categoria);
    int encontrados  = 0;

    const NoHash *atual = h->tabela[idx];

    printf("  Resultados para categoria \"%s\" (bucket %u):\n", categoria, idx);
    printf("  %s\n", "------------------------------------------------------------");

    while (atual != NULL) {
        /* Compara a categoria do nó com a categoria buscada */
        if (strcmp(atual->dado.categoria, categoria) == 0) {
            const char *prio_str;
            switch (atual->dado.prioridade) {
                case 3:  prio_str = "Alta";  break;
                case 2:  prio_str = "Media"; break;
                default: prio_str = "Baixa"; break;
            }
            printf("  [%03d] %-20s %-15s %-10s %4d min  %s\n",
                   atual->dado.id,
                   atual->dado.cliente,
                   atual->dado.categoria,
                   prio_str,
                   atual->dado.tempo_estimado,
                   atual->dado.status);
            encontrados++;
        }
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("  Nenhum atendimento encontrado para a categoria \"%s\".\n", categoria);
    } else {
        printf("  %d registro(s) encontrado(s).\n", encontrados);
    }

    return encontrados;
}

/* ------------------------------------------------------------------
 * hash_imprimir_tabela
 * Exibe todos os buckets ocupados e seus atendimentos.
 * Útil para visualizar a distribuição da tabela hash.
 * ------------------------------------------------------------------ */
void hash_imprimir_tabela(const TabelaHash *h) {
    int i;
    printf("  === Tabela Hash (tamanho: %d) ===\n", HASH_TAMANHO);
    for (i = 0; i < HASH_TAMANHO; i++) {
        if (h->tabela[i] != NULL) {
            printf("  Bucket [%02d]: ", i);
            const NoHash *atual = h->tabela[i];
            while (atual != NULL) {
                printf("[ID:%d|%s] ", atual->dado.id, atual->dado.categoria);
                if (atual->proximo != NULL) printf("-> ");
                atual = atual->proximo;
            }
            printf("\n");
        }
    }
    printf("  Total de elementos: %d\n", h->total);
}

/* ------------------------------------------------------------------
 * hash_destruir
 * Percorre todos os buckets e libera cada nó da cadeia.
 * ------------------------------------------------------------------ */
void hash_destruir(TabelaHash *h) {
    int i;
    for (i = 0; i < HASH_TAMANHO; i++) {
        NoHash *atual = h->tabela[i];
        while (atual != NULL) {
            NoHash *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        h->tabela[i] = NULL;
    }
    h->total = 0;
}
