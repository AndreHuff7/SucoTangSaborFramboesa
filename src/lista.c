/*
 * lista.c
 * =======
 * Implementação da Lista Encadeada Simples para histórico de atendimentos.
 *
 * Estratégia:
 *   - `cabeca` aponta para o primeiro nó (início da lista).
 *   - `cauda`  aponta para o último nó (permite inserção em O(1)).
 *   - Inserção sempre ocorre no final para preservar ordem cronológica.
 *   - A busca é sequencial (O(n)) pois não há garantia de ordenação por nome.
 *
 * Nota sobre lista_atualizar_status:
 *   Quando um atendimento é removido da fila, seu status muda para "atendido".
 *   Esta função percorre a lista pelo ID e sincroniza esse campo no histórico.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

/* ------------------------------------------------------------------
 * lista_inicializar
 * Prepara a estrutura para uso.
 * ------------------------------------------------------------------ */
void lista_inicializar(Lista *l) {
    l->cabeca   = NULL;
    l->cauda    = NULL;
    l->tamanho  = 0;
}

/* ------------------------------------------------------------------
 * lista_inserir
 * Aloca um novo nó, copia os dados e encadeia no final da lista.
 * Retorna 1 em sucesso, 0 em caso de falha.
 * ------------------------------------------------------------------ */
int lista_inserir(Lista *l, Atendimento a) {
    NoLista *novo = (NoLista *)malloc(sizeof(NoLista));
    if (!novo) {
        fprintf(stderr, "[LISTA] Erro: falha ao alocar memoria.\n");
        return 0;
    }

    novo->dado    = a;
    novo->proximo = NULL;

    if (l->cabeca == NULL) {
        /* Lista estava vazia: novo nó é cabeça e cauda ao mesmo tempo */
        l->cabeca = novo;
        l->cauda  = novo;
    } else {
        /* Encadeia o novo nó após a cauda atual */
        l->cauda->proximo = novo;
        l->cauda          = novo;
    }

    l->tamanho++;
    return 1;
}

/* ------------------------------------------------------------------
 * lista_imprimir
 * Exibe todos os atendimentos do histórico com seus campos principais.
 * ------------------------------------------------------------------ */
void lista_imprimir(const Lista *l) {
    if (l->cabeca == NULL) {
        printf("  [Historico vazio]\n");
        return;
    }

    printf("  %-5s %-20s %-15s %-10s %-8s %-12s\n",
           "ID", "Cliente", "Categoria", "Prioridade", "Tempo", "Status");
    printf("  %s\n",
           "------------------------------------------------------------------------");

    const NoLista *atual = l->cabeca;
    while (atual != NULL) {
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
        atual = atual->proximo;
    }
    printf("  Total de registros: %d\n", l->tamanho);
}

/* ------------------------------------------------------------------
 * lista_buscar_por_cliente
 * Percorre todos os nós comparando o campo `cliente` com a string
 * fornecida. Usa strstr para busca de substring (case-sensitive).
 * Imprime os registros encontrados e retorna a contagem.
 *
 * Complexidade: O(n) — busca sequencial, inevitável em lista encadeada
 * sem índice secundário ordenado por nome.
 * ------------------------------------------------------------------ */
int lista_buscar_por_cliente(const Lista *l, const char *nome) {
    if (l->cabeca == NULL) {
        printf("  [Historico vazio]\n");
        return 0;
    }

    int encontrados = 0;
    const NoLista *atual = l->cabeca;

    printf("  Resultados para cliente contendo \"%s\":\n", nome);
    printf("  %s\n", "------------------------------------------------------------");

    while (atual != NULL) {
        /* strstr retorna ponteiro não-nulo se 'nome' for substring de 'cliente' */
        if (strstr(atual->dado.cliente, nome) != NULL) {
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
        printf("  Nenhum atendimento encontrado para \"%s\".\n", nome);
    } else {
        printf("  %d registro(s) encontrado(s).\n", encontrados);
    }

    return encontrados;
}

/* ------------------------------------------------------------------
 * lista_atualizar_status
 * Busca um nó pelo ID e atualiza seu campo status.
 * Chamada após fila_desenfileirar para manter o histórico consistente.
 * ------------------------------------------------------------------ */
void lista_atualizar_status(Lista *l, int id, const char *novo_status) {
    NoLista *atual = l->cabeca;
    while (atual != NULL) {
        if (atual->dado.id == id) {
            strncpy(atual->dado.status, novo_status, TAM_STATUS - 1);
            atual->dado.status[TAM_STATUS - 1] = '\0';
            return;
        }
        atual = atual->proximo;
    }
}

/* ------------------------------------------------------------------
 * lista_destruir
 * Percorre todos os nós liberando memória. Reinicializa a estrutura.
 * ------------------------------------------------------------------ */
void lista_destruir(Lista *l) {
    NoLista *atual = l->cabeca;
    while (atual != NULL) {
        NoLista *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    lista_inicializar(l);
}
