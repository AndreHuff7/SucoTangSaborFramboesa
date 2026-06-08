/*
 * ordenacao.h
 * ===========
 * Interface pública do módulo de ordenação da Lista Encadeada.
 *
 * Implementa o algoritmo Merge Sort adaptado para listas encadeadas.
 *
 * Por que Merge Sort para lista encadeada?
 *   - É o algoritmo mais eficiente (O(n log n)) para listas encadeadas.
 *   - Não requer acesso aleatório a elementos (diferente de Quick Sort).
 *   - É estável: elementos com mesma prioridade mantêm ordem relativa.
 *   - Não precisa de array auxiliar — reorganiza apenas os ponteiros.
 *
 * Critério de ordenação:
 *   Primário:   `prioridade` DECRESCENTE (alta=3 vem antes de baixa=1)
 *   Secundário: `tempo_estimado` CRESCENTE (menor tempo tem preferência)
 */

#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "lista.h"

/*
 * Ordena a lista encadeada de histórico usando Merge Sort.
 * A lista é modificada in-place (apenas ponteiros são reorganizados).
 * Complexidade: O(n log n) no pior caso.
 */
void ordenar_lista(Lista *l);

#endif /* ORDENACAO_H */
