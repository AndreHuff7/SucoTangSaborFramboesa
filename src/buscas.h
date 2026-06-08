/*
 * buscas.h
 * ========
 * Interface pública do módulo de buscas e utilitários de vetor.
 *
 * Este módulo implementa os três métodos de busca sobre vetores de inteiros
 * para fins de comparação experimental:
 *
 *   1. Busca Sequencial  — O(n)       — funciona em qualquer vetor
 *   2. Busca Binária     — O(log n)   — requer vetor ordenado
 *   3. Busca na Árvore   — ver arvore.h — buscarNaArvore()
 *
 * Todas as funções de busca recebem um ponteiro `int *comparacoes` que é
 * INCREMENTADO a cada comparação realizada (não zerado aqui — o chamador
 * deve zerar antes de chamar se quiser contagem isolada).
 */

#ifndef BUSCAS_H
#define BUSCAS_H

/* ------------------------------------------------------------------ */
/* Buscas em vetor de inteiros                                         */
/* ------------------------------------------------------------------ */

/*
 * buscaSequencial
 * Percorre o vetor do índice 0 até (tamanho-1) comparando cada elemento
 * com `valor`. Conta cada comparação em *comparacoes.
 * Retorna o índice onde encontrou, ou -1 se não encontrou.
 */
int buscaSequencial(int vetor[], int tamanho, int valor, int *comparacoes);

/*
 * buscaBinaria
 * Requer vetor ordenado em ordem crescente.
 * A cada iteração compara o elemento do meio com `valor` e descarta
 * metade do intervalo. Conta cada comparação em *comparacoes.
 * Retorna o índice onde encontrou, ou -1 se não encontrou.
 */
int buscaBinaria(int vetor[], int tamanho, int valor, int *comparacoes);

/* ------------------------------------------------------------------ */
/* Utilitários de vetor                                                */
/* ------------------------------------------------------------------ */

/*
 * gerarVetorOrdenado
 * Aloca e retorna um vetor com `tamanho` inteiros ordenados: 1, 2, ..., n.
 * O chamador é responsável por liberar com free().
 * Retorna NULL em falha de alocação.
 */
int *gerarVetorOrdenado(int tamanho);

/*
 * gerarVetorEmbaralhado
 * Gera vetor ordenado e embaralha com Fisher-Yates.
 * srand() é chamado com semente fixa (42) para reprodutibilidade.
 * O chamador é responsável por liberar com free().
 * Retorna NULL em falha de alocação.
 */
int *gerarVetorEmbaralhado(int tamanho);

/*
 * ordenarVetor
 * Ordena vetor in-place em ordem crescente usando qsort da stdlib.
 */
void ordenarVetor(int vetor[], int tamanho);

#endif /* BUSCAS_H */
