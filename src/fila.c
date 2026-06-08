/*
 * fila.c
 * ======
 * Implementação da Fila Dinâmica (FIFO).
 *
 * Estratégia:
 *   - `frente`   aponta para o nó que será removido (dequeue).
 *   - `traseira` aponta para o nó onde o próximo elemento será inserido (enqueue).
 *   - Ao enfileirar: aloca novo nó, liga no final da lista e atualiza `traseira`.
 *   - Ao desenfileirar: salva dados do nó da frente, avança `frente`, libera o nó.
 *
 * Complexidade:
 *   - enqueue: O(1)
 *   - dequeue: O(1)
 *   - imprimir: O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

/* ------------------------------------------------------------------
 * fila_inicializar
 * Prepara a estrutura para uso, zerando ponteiros e tamanho.
 * Deve ser chamada antes de qualquer outra operação.
 * ------------------------------------------------------------------ */
void fila_inicializar(Fila *f) {
    f->frente   = NULL;
    f->traseira = NULL;
    f->tamanho  = 0;
}

/* ------------------------------------------------------------------
 * fila_vazia
 * Retorna 1 (verdadeiro) se não há elementos na fila.
 * ------------------------------------------------------------------ */
int fila_vazia(const Fila *f) {
    return (f->frente == NULL);
}

/* ------------------------------------------------------------------
 * fila_enfileirar
 * Aloca um novo nó, copia o atendimento e o insere no final da fila.
 * Retorna 1 em sucesso, 0 se malloc falhar.
 * ------------------------------------------------------------------ */
int fila_enfileirar(Fila *f, Atendimento a) {
    /* Aloca memória para o novo nó */
    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
    if (!novo) {
        fprintf(stderr, "[FILA] Erro: falha ao alocar memoria.\n");
        return 0;
    }

    /* Copia os dados do atendimento para dentro do nó */
    novo->dado    = a;
    novo->proximo = NULL; /* Novo nó será o último, sem próximo */

    if (fila_vazia(f)) {
        /* Fila vazia: frente e traseira apontam para o mesmo nó */
        f->frente   = novo;
        f->traseira = novo;
    } else {
        /* Encadeia o novo nó após o atual último nó */
        f->traseira->proximo = novo;
        f->traseira          = novo;
    }

    f->tamanho++;
    return 1;
}

/* ------------------------------------------------------------------
 * fila_desenfileirar
 * Remove o nó da frente, copia os dados para *saida e marca o status
 * do atendimento como "atendido". Libera a memória do nó removido.
 * Retorna 1 em sucesso, 0 se a fila estiver vazia.
 * ------------------------------------------------------------------ */
int fila_desenfileirar(Fila *f, Atendimento *saida) {
    if (fila_vazia(f)) {
        printf("[FILA] Fila vazia. Nenhum atendimento a processar.\n");
        return 0;
    }

    /* Guarda ponteiro para o nó que será removido */
    NoFila *removido = f->frente;

    /* Copia os dados para o ponteiro de saída antes de liberar */
    *saida = removido->dado;

    /* Marca o status do atendimento como "atendido" */
    strncpy(saida->status, "atendido", TAM_STATUS - 1);
    saida->status[TAM_STATUS - 1] = '\0';

    /* Avança a frente da fila para o próximo nó */
    f->frente = removido->proximo;

    /* Se a fila ficou vazia, a traseira também deve ser NULL */
    if (f->frente == NULL) {
        f->traseira = NULL;
    }

    /* Libera a memória do nó removido */
    free(removido);
    f->tamanho--;
    return 1;
}

/* ------------------------------------------------------------------
 * fila_imprimir
 * Percorre a fila do início ao fim imprimindo cada atendimento.
 * Não remove nenhum elemento (operação read-only).
 * ------------------------------------------------------------------ */
void fila_imprimir(const Fila *f) {
    if (fila_vazia(f)) {
        printf("  [Fila de espera vazia]\n");
        return;
    }

    printf("  %-5s %-20s %-15s %-10s %-8s\n",
           "ID", "Cliente", "Categoria", "Prioridade", "Tempo(min)");
    printf("  %s\n", "------------------------------------------------------------");

    const NoFila *atual = f->frente;
    int posicao = 1;
    while (atual != NULL) {
        /* Converte prioridade numérica para texto legível */
        const char *prio_str;
        switch (atual->dado.prioridade) {
            case 3:  prio_str = "Alta";  break;
            case 2:  prio_str = "Media"; break;
            default: prio_str = "Baixa"; break;
        }
        printf("  %d) [ID:%d] %-20s %-15s %-10s %d min\n",
               posicao,
               atual->dado.id,
               atual->dado.cliente,
               atual->dado.categoria,
               prio_str,
               atual->dado.tempo_estimado);
        atual = atual->proximo;
        posicao++;
    }
    printf("  Total na fila: %d\n", f->tamanho);
}

/* ------------------------------------------------------------------
 * fila_destruir
 * Percorre todos os nós e libera a memória alocada dinamicamente.
 * Deve ser chamada ao encerrar o programa para evitar memory leaks.
 * ------------------------------------------------------------------ */
void fila_destruir(Fila *f) {
    NoFila *atual = f->frente;
    while (atual != NULL) {
        NoFila *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    /* Reinicializa a estrutura para estado seguro */
    fila_inicializar(f);
}
