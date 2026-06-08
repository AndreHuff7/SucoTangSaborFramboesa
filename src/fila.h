/*
 * fila.h
 * ======
 * Interface pública da Fila Dinâmica (FIFO - First In, First Out).
 *
 * A fila controla a ordem de atendimento: o primeiro a chegar é o primeiro
 * a ser atendido. Internamente, cada elemento é um nó alocado dinamicamente
 * (heap), armazenando uma CÓPIA do Atendimento.
 *
 * A implementação usa dois ponteiros (frente e traseira) para garantir que
 * tanto enqueue quanto dequeue sejam executados em O(1).
 */

#ifndef FILA_H
#define FILA_H

#include "atendimento.h"

/* ------------------------------------------------------------------ */
/* Nó interno da fila — não exposto diretamente ao usuário do TAD      */
/* ------------------------------------------------------------------ */
typedef struct NoFila {
    Atendimento dado;       /* Cópia do atendimento armazenado neste nó */
    struct NoFila *proximo; /* Ponteiro para o próximo nó na fila        */
} NoFila;

/* ------------------------------------------------------------------ */
/* Estrutura da Fila                                                    */
/* ------------------------------------------------------------------ */
typedef struct {
    NoFila *frente;  /* Ponteiro para o primeiro elemento (quem sai)  */
    NoFila *traseira;/* Ponteiro para o último elemento (quem entrou) */
    int     tamanho; /* Contagem de elementos para consultas em O(1)  */
} Fila;

/* ------------------------------------------------------------------ */
/* Protótipos das funções públicas                                      */
/* ------------------------------------------------------------------ */

/* Inicializa a fila, zerando todos os ponteiros e o tamanho */
void fila_inicializar(Fila *f);

/* Enfileira uma cópia do atendimento no final da fila. Retorna 1 em
   sucesso ou 0 em caso de falha de alocação de memória. */
int fila_enfileirar(Fila *f, Atendimento a);

/* Remove o elemento da frente da fila, copia seus dados para *saida e
   altera o status do atendimento para "atendido".
   Retorna 1 em sucesso ou 0 se a fila estiver vazia. */
int fila_desenfileirar(Fila *f, Atendimento *saida);

/* Retorna 1 se a fila estiver vazia, 0 caso contrário */
int fila_vazia(const Fila *f);

/* Imprime todos os elementos da fila sem removê-los */
void fila_imprimir(const Fila *f);

/* Libera todos os nós alocados dinamicamente e reinicializa a fila */
void fila_destruir(Fila *f);

#endif /* FILA_H */
