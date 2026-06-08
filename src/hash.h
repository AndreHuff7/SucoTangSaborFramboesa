/*
 * hash.h
 * ======
 * Interface pública da Tabela Hash com Encadeamento por Colisão.
 *
 * A tabela hash permite acesso médio O(1) para inserção e busca.
 * A chave de hash é a `categoria` do atendimento (string).
 *
 * Tratamento de colisões — Encadeamento Separado (Separate Chaining):
 *   Cada posição da tabela é a cabeça de uma lista encadeada.
 *   Quando duas categorias distintas mapeiam para o mesmo índice (colisão),
 *   ambos os atendimentos são encadeados na mesma lista.
 *   Isso permite inserção sempre em O(1) e busca O(1) no caso médio.
 *
 * Função de hash — djb2:
 *   Algoritmo de hash de strings amplamente testado. Propaga bits
 *   eficientemente: hash = hash * 33 + c para cada caractere 'c'.
 *   O resultado é reduzido ao intervalo [0, HASH_TAMANHO) com módulo.
 *
 * Estrutura interna:
 *   tabela[HASH_TAMANHO] — array de ponteiros para NosHash.
 *   Cada NosHash guarda uma cópia do Atendimento e aponta para o próximo
 *   elemento na mesma posição (cadeia de colisões).
 */

#ifndef HASH_H
#define HASH_H

#include "atendimento.h"

/* Tamanho da tabela hash (número primo para melhor distribuição) */
#define HASH_TAMANHO 31

/* ------------------------------------------------------------------ */
/* Nó da lista de colisão interna                                      */
/* ------------------------------------------------------------------ */
typedef struct NoHash {
    Atendimento dado;     /* Cópia do atendimento                      */
    struct NoHash *proximo; /* Próximo nó na mesma posição (colisão)   */
} NoHash;

/* ------------------------------------------------------------------ */
/* Estrutura da Tabela Hash                                            */
/* ------------------------------------------------------------------ */
typedef struct {
    NoHash *tabela[HASH_TAMANHO]; /* Array de cabeças de listas         */
    int     total;                /* Contagem total de elementos        */
} TabelaHash;

/* ------------------------------------------------------------------ */
/* Protótipos das funções públicas                                      */
/* ------------------------------------------------------------------ */

/* Inicializa todos os buckets com NULL */
void hash_inicializar(TabelaHash *h);

/* Insere uma cópia do atendimento no bucket correspondente à categoria.
   Retorna 1 em sucesso, 0 em falha de alocação. */
int hash_inserir(TabelaHash *h, Atendimento a);

/* Busca e imprime todos os atendimentos de uma determinada categoria.
   Retorna o número de atendimentos encontrados. */
int hash_buscar_por_categoria(const TabelaHash *h, const char *categoria);

/* Exibe estatísticas da tabela (ocupação dos buckets) — útil para debug */
void hash_imprimir_tabela(const TabelaHash *h);

/* Libera todos os nós de todos os buckets */
void hash_destruir(TabelaHash *h);

#endif /* HASH_H */
