/*
 * arvore.c
 * ========
 * Implementação da Árvore Binária de Busca (ABB).
 *
 * Todas as operações recursivas são implementadas como funções estáticas
 * (internas) que recebem e retornam ponteiros para nós. As funções públicas
 * são wrappers que chamam as recursivas a partir da raiz.
 *
 * Inserção (arvore_inserir_rec):
 *   Compara o id com o nó atual. Se menor, desce à esquerda; se maior,
 *   à direita. Se NULL, cria um novo nó nessa posição. Retorna o ponteiro
 *   do nó para manter o encadeamento correto.
 *
 * Busca (arvore_buscar_rec):
 *   Mesma lógica de navegação, mas retorna o dado ao encontrar id igual.
 *
 * Destruição (arvore_destruir_rec):
 *   Percurso pós-ordem: destrói filhos antes de liberar o nó pai, evitando
 *   perda de referências.
 */

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

/* ------------------------------------------------------------------
 * arvore_inicializar
 * ------------------------------------------------------------------ */
void arvore_inicializar(Arvore *a) {
    a->raiz    = NULL;
    a->tamanho = 0;
}

/* ------------------------------------------------------------------
 * arvore_criar_no (interno)
 * Aloca e inicializa um novo nó com o atendimento fornecido.
 * ------------------------------------------------------------------ */
static NoArvore *arvore_criar_no(Atendimento at) {
    NoArvore *novo = (NoArvore *)malloc(sizeof(NoArvore));
    if (!novo) {
        fprintf(stderr, "[ARVORE] Erro: falha ao alocar memoria.\n");
        return NULL;
    }
    novo->dado      = at;
    novo->esquerda  = NULL;
    novo->direita   = NULL;
    return novo;
}

/* ------------------------------------------------------------------
 * arvore_inserir_rec (interno)
 * Insere recursivamente e retorna o ponteiro do nó atual (ou novo).
 * O segundo parâmetro é usado para sinalizar se a inserção ocorreu.
 * ------------------------------------------------------------------ */
static NoArvore *arvore_inserir_rec(NoArvore *no, Atendimento at, int *inserido) {
    if (no == NULL) {
        /* Posição encontrada: cria o nó aqui */
        *inserido = 1;
        return arvore_criar_no(at);
    }

    if (at.id < no->dado.id) {
        /* id menor: desce para a subárvore esquerda */
        no->esquerda = arvore_inserir_rec(no->esquerda, at, inserido);
    } else if (at.id > no->dado.id) {
        /* id maior: desce para a subárvore direita */
        no->direita = arvore_inserir_rec(no->direita, at, inserido);
    } else {
        /* id duplicado: não insere */
        printf("[ARVORE] Aviso: ID %d ja existe na arvore.\n", at.id);
        *inserido = 0;
    }

    return no;
}

/* ------------------------------------------------------------------
 * arvore_inserir
 * Ponto de entrada público para inserção.
 * ------------------------------------------------------------------ */
int arvore_inserir(Arvore *a, Atendimento at) {
    int inserido = 0;
    a->raiz = arvore_inserir_rec(a->raiz, at, &inserido);
    if (inserido) {
        a->tamanho++;
    }
    return inserido;
}

/* ------------------------------------------------------------------
 * arvore_buscar_rec (interno)
 * Busca recursivamente pelo id. Retorna ponteiro para o dado ou NULL.
 * ------------------------------------------------------------------ */
static const Atendimento *arvore_buscar_rec(const NoArvore *no, int id) {
    if (no == NULL) {
        return NULL; /* id não encontrado */
    }

    if (id == no->dado.id) {
        return &no->dado; /* Encontrado! */
    } else if (id < no->dado.id) {
        return arvore_buscar_rec(no->esquerda, id);
    } else {
        return arvore_buscar_rec(no->direita, id);
    }
}

/* ------------------------------------------------------------------
 * arvore_buscar
 * Ponto de entrada público para busca por id.
 * ------------------------------------------------------------------ */
const Atendimento *arvore_buscar(const Arvore *a, int id) {
    return arvore_buscar_rec(a->raiz, id);
}

/* ------------------------------------------------------------------
 * arvore_imprimir_inordem_rec (interno)
 * Percurso em-ordem (esquerda -> raiz -> direita).
 * Como a ABB é ordenada por id, resulta em impressão em ordem crescente de id.
 * ------------------------------------------------------------------ */
static void arvore_imprimir_inordem_rec(const NoArvore *no) {
    if (no == NULL) return;

    arvore_imprimir_inordem_rec(no->esquerda);

    const char *prio_str;
    switch (no->dado.prioridade) {
        case 3:  prio_str = "Alta";  break;
        case 2:  prio_str = "Media"; break;
        default: prio_str = "Baixa"; break;
    }
    printf("  [%03d] %-20s %-15s %-10s %4d min  %s\n",
           no->dado.id,
           no->dado.cliente,
           no->dado.categoria,
           prio_str,
           no->dado.tempo_estimado,
           no->dado.status);

    arvore_imprimir_inordem_rec(no->direita);
}

/* ------------------------------------------------------------------
 * arvore_imprimir_inordem
 * ------------------------------------------------------------------ */
void arvore_imprimir_inordem(const Arvore *a) {
    if (a->raiz == NULL) {
        printf("  [Arvore vazia]\n");
        return;
    }
    printf("  %-5s %-20s %-15s %-10s %-8s %-12s\n",
           "ID", "Cliente", "Categoria", "Prioridade", "Tempo", "Status");
    printf("  %s\n",
           "------------------------------------------------------------------------");
    arvore_imprimir_inordem_rec(a->raiz);
}

/* ------------------------------------------------------------------
 * arvore_destruir_rec (interno)
 * Pós-ordem: libera os filhos antes do nó pai.
 * ------------------------------------------------------------------ */
static void arvore_destruir_rec(NoArvore *no) {
    if (no == NULL) return;
    arvore_destruir_rec(no->esquerda);
    arvore_destruir_rec(no->direita);
    free(no);
}

/* ------------------------------------------------------------------
 * arvore_destruir
 * ------------------------------------------------------------------ */
void arvore_destruir(Arvore *a) {
    arvore_destruir_rec(a->raiz);
    arvore_inicializar(a);
}

/* ==================================================================
 * IMPLEMENTAÇÃO DA ABB DE INTEIROS
 *
 * Todas as operações são iterativas para suportar até 1.000.000 de
 * elementos inseridos em ordem (árvore degenerada com altura = N),
 * sem causar estouro da pilha de chamadas recursivas.
 * ================================================================== */

/* ------------------------------------------------------------------
 * arvoreInt_inicializar
 * ------------------------------------------------------------------ */
void arvoreInt_inicializar(ArvoreInt *a) {
    a->raiz    = NULL;
    a->tamanho = 0;
}

/* ------------------------------------------------------------------
 * arvoreInt_inserir
 * Inserção ITERATIVA: percorre a árvore com um ponteiro até encontrar
 * NULL, então aloca e encadeia o novo nó sem usar recursão.
 * ------------------------------------------------------------------ */
int arvoreInt_inserir(ArvoreInt *a, int valor) {
    NoInt *novo = (NoInt *)malloc(sizeof(NoInt));
    if (!novo) {
        fprintf(stderr, "[ARVORE_INT] Erro: falha ao alocar memoria.\n");
        return 0;
    }
    novo->valor    = valor;
    novo->esquerda = NULL;
    novo->direita  = NULL;

    if (a->raiz == NULL) {
        a->raiz = novo;
        a->tamanho++;
        return 1;
    }

    NoInt *atual = a->raiz;
    NoInt *pai   = NULL;
    int    vai_esq = 0;

    while (atual != NULL) {
        pai = atual;
        if (valor < atual->valor) {
            vai_esq = 1;
            atual   = atual->esquerda;
        } else if (valor > atual->valor) {
            vai_esq = 0;
            atual   = atual->direita;
        } else {
            /* Valor duplicado — não insere */
            free(novo);
            return 0;
        }
    }

    if (vai_esq) pai->esquerda = novo;
    else         pai->direita  = novo;

    a->tamanho++;
    return 1;
}

/* ------------------------------------------------------------------
 * buscarNaArvore
 * Busca ITERATIVA com contagem de comparações.
 * Cada vez que o valor do nó é acessado conta como 1 comparação.
 * Retorna 1 se encontrou, 0 caso contrário.
 * ------------------------------------------------------------------ */
int buscarNaArvore(NoInt *raiz, int valor, int *comparacoes) {
    NoInt *atual = raiz;
    while (atual != NULL) {
        (*comparacoes)++;
        if (valor == atual->valor) {
            return 1;
        } else if (valor < atual->valor) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    return 0;
}

/* ------------------------------------------------------------------
 * alturaArvore
 * Calcula a altura usando BFS (busca em largura) com fila alocada
 * dinamicamente. Evita recursão para suportar árvores degeneradas.
 *
 * Complexidade: O(n) tempo, O(largura_max) espaço.
 * Para árvore degenerada (pior caso): largura_max = 1, logo O(1) espaço.
 * Para árvore balanceada com N nós: largura_max ≈ N/2.
 * ------------------------------------------------------------------ */
int alturaArvore(NoInt *raiz) {
    if (raiz == NULL) return 0;

    /* Fila dinâmica: começa com tamanho 1024, dobra quando necessário */
    int cap    = 1024;
    int inicio = 0;
    int fim    = 0;

    NoInt **fila = (NoInt **)malloc((size_t)cap * sizeof(NoInt *));
    if (!fila) return -1; /* falha de alocação */

    fila[fim++] = raiz;

    int altura = 0;

    while (inicio < fim) {
        int nos_no_nivel = fim - inicio;
        altura++;

        /* Processa todos os nós do nível atual */
        int i;
        for (i = 0; i < nos_no_nivel; i++) {
            NoInt *no = fila[inicio++];

            /* Garante espaço para 2 filhos */
            if (fim + 2 > cap) {
                cap *= 2;
                NoInt **nova_fila = (NoInt **)realloc(fila,
                                        (size_t)cap * sizeof(NoInt *));
                if (!nova_fila) { free(fila); return -1; }
                fila = nova_fila;
            }

            if (no->esquerda) fila[fim++] = no->esquerda;
            if (no->direita)  fila[fim++] = no->direita;
        }
    }

    free(fila);
    return altura;
}

/* ------------------------------------------------------------------
 * arvoreInt_destruir
 * Destrói a árvore em O(n) tempo e O(1) espaço usando o algoritmo
 * de rotação à direita (Day-Stout-Warren adaptado para destruição):
 *
 *   Enquanto curr != NULL:
 *     se não tem filho esquerdo: libera curr, avança para direita
 *     senão: rotaciona à direita (sobe o filho esquerdo) — sem liberar
 *
 * Isso converte a árvore em uma "videira" (lista à direita) enquanto
 * libera os nós, sem usar pilha auxiliar.
 * ------------------------------------------------------------------ */
void arvoreInt_destruir(ArvoreInt *a) {
    NoInt *curr = a->raiz;
    while (curr != NULL) {
        if (curr->esquerda == NULL) {
            /* Nó sem filho esquerdo: pode liberar e avançar */
            NoInt *prox = curr->direita;
            free(curr);
            curr = prox;
        } else {
            /* Rotação à direita: filho esquerdo sobe */
            NoInt *esq    = curr->esquerda;
            curr->esquerda = esq->direita;
            esq->direita   = curr;
            curr           = esq;
        }
    }
    arvoreInt_inicializar(a);
}
