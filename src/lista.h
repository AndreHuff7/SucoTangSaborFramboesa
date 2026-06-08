/*
 * lista.h
 * =======
 * Interface pública da Lista Encadeada Simples.
 *
 * A lista mantém o HISTÓRICO COMPLETO de todos os atendimentos cadastrados
 * no sistema, independentemente do status (aberto, atendido, cancelado).
 * Cada nó é alocado dinamicamente e armazena uma CÓPIA do Atendimento.
 *
 * Permite inserção no final, impressão e busca sequencial por nome de cliente.
 * O módulo de ordenação (ordenacao.h) pode reorganizar os nós desta lista.
 */

#ifndef LISTA_H
#define LISTA_H

#include "atendimento.h"

/* ------------------------------------------------------------------ */
/* Nó da lista encadeada                                               */
/* ------------------------------------------------------------------ */
typedef struct NoLista {
    Atendimento dado;        /* Cópia do atendimento armazenado neste nó */
    struct NoLista *proximo; /* Ponteiro para o próximo nó da lista       */
} NoLista;

/* ------------------------------------------------------------------ */
/* Estrutura da Lista                                                   */
/* ------------------------------------------------------------------ */
typedef struct {
    NoLista *cabeca;  /* Ponteiro para o primeiro nó da lista  */
    NoLista *cauda;   /* Ponteiro para o último nó (inserção O(1)) */
    int      tamanho; /* Número atual de elementos na lista    */
} Lista;

/* ------------------------------------------------------------------ */
/* Protótipos das funções públicas                                      */
/* ------------------------------------------------------------------ */

/* Inicializa a lista zerada */
void lista_inicializar(Lista *l);

/* Insere uma cópia do atendimento no final da lista. Retorna 1 em sucesso,
   0 em caso de falha de alocação. */
int lista_inserir(Lista *l, Atendimento a);

/* Imprime todo o histórico de atendimentos da lista */
void lista_imprimir(const Lista *l);

/* Busca sequencial (O(n)) pelo nome do cliente.
   Imprime todos os registros cujo campo `cliente` contenha a substring
   fornecida (case-sensitive). Retorna o número de correspondências. */
int lista_buscar_por_cliente(const Lista *l, const char *nome);

/* Atualiza o status de um atendimento no histórico pelo seu ID.
   Usado para sincronizar o status após um dequeue da fila. */
void lista_atualizar_status(Lista *l, int id, const char *novo_status);

/* Libera todos os nós e reinicializa a lista */
void lista_destruir(Lista *l);

#endif /* LISTA_H */
