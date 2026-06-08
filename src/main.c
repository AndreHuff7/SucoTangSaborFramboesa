/*
 * main.c
 * ======
 * Ponto de entrada do Sistema de Controle de Atendimentos.
 *
 * Responsabilidades do main.c:
 *   - Inicializar todas as estruturas de dados.
 *   - Exibir o menu interativo e capturar a escolha do usuário.
 *   - Chamar as funções dos módulos correspondentes (lógica NÃO está aqui).
 *   - Liberar toda a memória alocada antes de encerrar (opção 0).
 *
 * Integração das estruturas:
 *   Cadastrar (opção 1) insere o mesmo atendimento em QUATRO estruturas:
 *     - Fila    → controla a ordem de atendimento (FIFO)
 *     - Lista   → mantém o histórico completo
 *     - Árvore  → permite busca rápida por ID
 *     - Hash    → agrupa atendimentos por categoria
 *
 *   Atender (opção 2) remove da Fila e atualiza o status na Lista.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   /* clock(), clock_t, CLOCKS_PER_SEC — medição de tempo */

#include "atendimento.h"
#include "fila.h"
#include "lista.h"
#include "ordenacao.h"
#include "arvore.h"
#include "hash.h"
#include "buscas.h" /* buscaSequencial, buscaBinaria, gerarVetor*, ordenarVetor */

/* ================================================================== */
/* Funções auxiliares do main (limpeza de tela, leitura segura)        */
/* ================================================================== */

/* Lê uma string do stdin com tamanho máximo, evitando buffer overflow.
   Remove o '\n' final se presente. */
static void ler_string(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin)) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        }
    }
}

/* Lê um inteiro do stdin de forma segura, descartando entradas inválidas */
static int ler_inteiro(void) {
    int valor;
    char buffer[32];
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &valor) == 1) {
                return valor;
            }
        }
        printf("  Entrada invalida. Digite um numero inteiro: ");
    }
}

/* ================================================================== */
/* Funções de cada opção do menu — mantêm o main.c limpo               */
/* ================================================================== */

/*
 * menu_cadastrar
 * Coleta os dados do novo atendimento via stdin, valida os campos
 * obrigatórios e insere nas quatro estruturas de dados.
 */
static void menu_cadastrar(Fila *fila, Lista *lista, Arvore *arvore,
                            TabelaHash *hash, int *proximo_id) {
    Atendimento novo;

    novo.id = *proximo_id;

    printf("\n  --- Cadastrar Novo Atendimento (ID: %d) ---\n", novo.id);

    printf("  Nome do cliente: ");
    ler_string(novo.cliente, TAM_CLIENTE);
    if (strlen(novo.cliente) == 0) {
        printf("  [Erro] Nome do cliente nao pode ser vazio.\n");
        return;
    }

    printf("  Categoria (ex: Suporte, Financeiro, Vendas): ");
    ler_string(novo.categoria, TAM_CATEGORIA);
    if (strlen(novo.categoria) == 0) {
        printf("  [Erro] Categoria nao pode ser vazia.\n");
        return;
    }

    printf("  Prioridade (1=Baixa, 2=Media, 3=Alta): ");
    novo.prioridade = ler_inteiro();
    if (novo.prioridade < 1 || novo.prioridade > 3) {
        printf("  [Erro] Prioridade invalida. Use 1, 2 ou 3.\n");
        return;
    }

    printf("  Tempo estimado (minutos): ");
    novo.tempo_estimado = ler_inteiro();
    if (novo.tempo_estimado <= 0) {
        printf("  [Erro] Tempo estimado deve ser maior que zero.\n");
        return;
    }

    /* Status inicial é sempre "aberto" */
    strncpy(novo.status, "aberto", TAM_STATUS - 1);
    novo.status[TAM_STATUS - 1] = '\0';

    /* Insere nas quatro estruturas */
    int ok = 1;
    ok &= fila_enfileirar(fila, novo);
    ok &= lista_inserir(lista, novo);
    ok &= arvore_inserir(arvore, novo);
    ok &= hash_inserir(hash, novo);

    if (ok) {
        (*proximo_id)++;
        printf("\n  [OK] Atendimento #%d de '%s' cadastrado com sucesso!\n",
               novo.id, novo.cliente);
    } else {
        printf("\n  [Erro] Falha ao cadastrar atendimento (memoria insuficiente).\n");
    }
}

/*
 * menu_atender_proximo
 * Remove o próximo da fila e atualiza o status no histórico (lista).
 */
static void menu_atender_proximo(Fila *fila, Lista *lista) {
    Atendimento atendido;

    printf("\n  --- Atender Proximo da Fila ---\n");

    if (!fila_desenfileirar(fila, &atendido)) {
        return; /* mensagem de fila vazia já foi impressa */
    }

    /* Sincroniza o status "atendido" no histórico da lista */
    lista_atualizar_status(lista, atendido.id, "atendido");

    printf("  [OK] Atendimento #%d do cliente '%s' marcado como ATENDIDO.\n",
           atendido.id, atendido.cliente);
    printf("       Categoria: %s | Prioridade: %d | Tempo: %d min\n",
           atendido.categoria, atendido.prioridade, atendido.tempo_estimado);
}

/*
 * menu_exibir_estatisticas
 * Percorre a lista de histórico e compila estatísticas agregadas.
 * A lógica de cálculo fica nesta função, não exposta aos módulos TAD.
 */
static void menu_exibir_estatisticas(const Lista *lista, const Fila *fila) {
    printf("\n  === Estatisticas do Sistema ===\n");

    if (lista->cabeca == NULL) {
        printf("  Nenhum atendimento registrado ainda.\n");
        return;
    }

    int total       = 0;
    int atendidos   = 0;
    int cancelados  = 0;
    int abertos     = 0;
    int prio_baixa  = 0;
    int prio_media  = 0;
    int prio_alta   = 0;
    int tempo_total = 0;

    const NoLista *atual = lista->cabeca;
    while (atual != NULL) {
        total++;
        tempo_total += atual->dado.tempo_estimado;

        if (strcmp(atual->dado.status, "atendido") == 0)   atendidos++;
        else if (strcmp(atual->dado.status, "cancelado") == 0) cancelados++;
        else                                                abertos++;

        switch (atual->dado.prioridade) {
            case 3: prio_alta++;  break;
            case 2: prio_media++; break;
            default: prio_baixa++; break;
        }

        atual = atual->proximo;
    }

    printf("  +------------------------------------------+\n");
    printf("  | Total de atendimentos registrados: %5d  |\n", total);
    printf("  | Atendidos:                         %5d  |\n", atendidos);
    printf("  | Abertos (na fila ou pendentes):    %5d  |\n", abertos);
    printf("  | Cancelados:                        %5d  |\n", cancelados);
    printf("  +------------------------------------------+\n");
    printf("  | Por prioridade:                           |\n");
    printf("  |   Alta  (3): %5d                        |\n", prio_alta);
    printf("  |   Media (2): %5d                        |\n", prio_media);
    printf("  |   Baixa (1): %5d                        |\n", prio_baixa);
    printf("  +------------------------------------------+\n");
    printf("  | Tempo total estimado:          %5d min  |\n", tempo_total);
    if (total > 0) {
        printf("  | Tempo medio por atendimento:   %5d min  |\n", tempo_total / total);
    }
    printf("  | Atendimentos aguardando na fila: %4d    |\n", fila->tamanho);
    printf("  +------------------------------------------+\n");
}

/* ================================================================== */
/* Exibição do menu principal                                          */
/* ================================================================== */
static void exibir_menu(void) {
    printf("\n");
    printf("  +----------------------------------------------+\n");
    printf("  |     SISTEMA DE CONTROLE DE ATENDIMENTOS      |\n");
    printf("  +----------------------------------------------+\n");
    printf("  |  1 - Cadastrar atendimento                   |\n");
    printf("  |  2 - Atender proximo da fila                 |\n");
    printf("  |  3 - Listar fila de espera                   |\n");
    printf("  |  4 - Listar historico completo               |\n");
    printf("  |  5 - Buscar por nome do cliente              |\n");
    printf("  |  6 - Buscar por ID (Arvore BST)              |\n");
    printf("  |  7 - Buscar por Categoria (Hash)             |\n");
    printf("  |  8 - Ordenar historico por prioridade        |\n");
    printf("  |  9 - Exibir estatisticas                     |\n");
    printf("  | 10 - Executar experimentos de busca          |\n");
    printf("  |  0 - Sair                                    |\n");
    printf("  +----------------------------------------------+\n");
    printf("  Opcao: ");
}

/* ==================================================================
 * executar_experimentos
 * ==============================================================
 * Executa automaticamente os cenários de comparação entre:
 *   - Busca Sequencial  (sobre vetor)
 *   - Busca Binária     (sobre vetor ordenado)
 *   - Busca na Árvore   (ABB de inteiros)
 *
 * Cenários:
 *   100 elementos ordenados
 *   100 elementos embaralhados
 *   1.000.000 elementos ordenados
 *   1.000.000 elementos embaralhados
 *
 * Para cada cenário, busca por:
 *   valor próximo do início  (pos = tamanho / 100)
 *   valor próximo do meio    (pos = tamanho / 2)
 *   valor próximo do fim     (pos = tamanho * 99 / 100)
 *   valor inexistente        (pos = tamanho + 1)
 *
 * Saída: stdout + resultados/resultados.csv
 * ================================================================== */
static void executar_experimentos(void) {
    /* Tamanhos dos experimentos */
    const int tamanhos[]  = {100, 100, 1000000, 1000000};
    const int embaralha[] = {  0,   1,       0,        1};
    const int num_exp     = 4;

    /* Cria diretório resultados/ se não existir (Windows) */
    system("if not exist resultados mkdir resultados");

    FILE *csv = fopen("resultados\\resultados.csv", "w");
    if (!csv) {
        /* Fallback: tenta no diretório atual */
        csv = fopen("resultados.csv", "w");
    }
    if (!csv) {
        printf("  [EXPERIMENTO] Erro: nao foi possivel criar o arquivo CSV.\n");
        return;
    }

    /* Cabeçalho CSV */
    fprintf(csv,
        "tamanho,tipo_dados,valor_buscado,posicao_busca,"
        "comp_sequencial,comp_binaria,comp_arvore,"
        "tempo_seq_us,tempo_bin_us,tempo_arv_us,"
        "altura_arvore,encontrado_seq,encontrado_bin,encontrado_arv\n");

    printf("\n  === Experimentos de Comparacao de Buscas ===\n");
    printf("  (Resultados salvos em resultados/resultados.csv)\n\n");

    int exp;
    for (exp = 0; exp < num_exp; exp++) {
        int   n    = tamanhos[exp];
        int   emb  = embaralha[exp];
        const char *tipo = emb ? "embaralhado" : "ordenado";

        printf("  >> Experimento: %d elementos %s\n", n, tipo);

        /* 1. Gera o vetor */
        int *vetor = emb ? gerarVetorEmbaralhado(n) : gerarVetorOrdenado(n);
        if (!vetor) {
            printf("     [ERRO] Falha ao alocar vetor. Pulando.\n");
            continue;
        }

        /* 2. Para busca binária o vetor deve estar ordenado */
        int *vetor_ord = (int *)malloc((size_t)n * sizeof(int));
        if (!vetor_ord) {
            printf("     [ERRO] Falha ao alocar vetor ordenado. Pulando.\n");
            free(vetor);
            continue;
        }
        /* Copia e ordena para a busca binária */
        int k;
        for (k = 0; k < n; k++) vetor_ord[k] = vetor[k];
        ordenarVetor(vetor_ord, n);

        /* 3. Constrói a ABB com os valores do vetor original.
         *
         * LIMITAÇÃO DOCUMENTADA:
         * Para dados ORDENADOS com n > 10.000, a ABB fica degenerada (altura = n).
         * Inserir n elementos em ordem crescente cria uma lista à direita e cada
         * inserção percorre toda a árvore → complexidade O(n²).
         * Para n = 1.000.000 isso seria ~5×10^11 operações (inviável em tempo prático).
         * Para n = 1.000.000.000 seriam ~5×10^17 operações (completamente impraticável,
         * além de ~24 GB de RAM necessária para os nós).
         * Nesses casos a ABB é pulada e -1 é registrado no CSV como indicador.
         */
        const int LIMITE_ABB_ORDENADA = 10000;
        int arvore_impraticavel = (!emb && n > LIMITE_ABB_ORDENADA);

        ArvoreInt arv;
        arvoreInt_inicializar(&arv);
        int altura = -1;

        if (!arvore_impraticavel) {
            for (k = 0; k < n; k++) {
                arvoreInt_inserir(&arv, vetor[k]);
            }
            altura = alturaArvore(arv.raiz);
        } else {
            printf("     [AVISO] ABB com dados ordenados (n=%d): construcao O(n^2) impraticavel.\n", n);
            printf("             Busca na arvore sera pulada. Ver CSV para detalhes.\n");
        }

        /* 5. Define os valores buscados */
        /* Os valores no vetor ordenado são 1..n, logo podemos usar
           índices diretamente como valores (vetor_ord[i] == i+1) */
        int valores[] = {
            vetor_ord[ n / 100 ],       /* perto do inicio  */
            vetor_ord[ n / 2   ],       /* perto do meio    */
            vetor_ord[ n * 99 / 100 ],  /* perto do fim     */
            n + 9999                    /* inexistente      */
        };
        const char *posicoes[] = {"inicio", "meio", "fim", "inexistente"};
        const int num_buscas = 4;

        /* 6. Executa buscas para cada valor */
        int b;
        for (b = 0; b < num_buscas; b++) {
            int valor = valores[b];

            int comp_seq = 0, comp_bin = 0, comp_arv = 0;
            clock_t t0, t1;
            double  us_seq, us_bin, us_arv;
            int r_arv = 0;

            /* Busca Sequencial (no vetor original) */
            t0 = clock();
            int r_seq = buscaSequencial(vetor, n, valor, &comp_seq);
            t1 = clock();
            us_seq = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

            /* Busca Binária (no vetor ordenado) */
            t0 = clock();
            int r_bin = buscaBinaria(vetor_ord, n, valor, &comp_bin);
            t1 = clock();
            us_bin = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

            /* Busca na Árvore */
            us_arv   = -1.0;
            comp_arv = -1;
            if (!arvore_impraticavel) {
                t0     = clock();
                r_arv  = buscarNaArvore(arv.raiz, valor, &comp_arv);
                t1     = clock();
                us_arv = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;
            }

            /* Impressão resumida no terminal */
            if (arvore_impraticavel) {
                printf("     [%s | seq:%d bin:%d] seq:%4d bin:%3d arv:N/A | %.1fus %.1fus N/A\n",
                       posicoes[b],
                       (r_seq >= 0), (r_bin >= 0),
                       comp_seq, comp_bin,
                       us_seq, us_bin);
            } else {
                printf("     [%s | %s] seq:%4d bin:%3d arv:%3d | %.1fus %.1fus %.1fus\n",
                       posicoes[b],
                       (r_seq >= 0 || r_bin >= 0 || r_arv) ? "ENCONTRADO" : "NAO FOUND",
                       comp_seq, comp_bin, comp_arv,
                       us_seq, us_bin, us_arv);
            }

            /* Grava linha no CSV */
            fprintf(csv,
                "%d,%s,%d,%s,%d,%d,%d,%.2f,%.2f,%.2f,%d,%d,%d,%d\n",
                n, tipo, valor, posicoes[b],
                comp_seq, comp_bin, comp_arv,
                us_seq, us_bin, us_arv,
                altura,
                (r_seq >= 0 ? 1 : 0),
                (r_bin >= 0 ? 1 : 0),
                r_arv);
        }

        if (altura >= 0) {
            printf("     Altura da arvore: %d\n", altura);
        }
        if (arvore_impraticavel) {
            printf("     LIMITACAO: ABB com dados ordenados de grande escala.\n");
            printf("       n=%d ordenado: construcao O(n^2) inviavel (~%.0f bi ops).\n",
                   n, (double)n * n / 2.0 / 1e9);
            printf("       n=1.000.000.000 ordenado: ~5x10^17 ops + ~24 GB RAM.\n");
            printf("       Solucao real: usar AVL/Red-Black para dados ordenados.\n");
        }

        /* 7. Libera memória do experimento */
        free(vetor);
        free(vetor_ord);
        arvoreInt_destruir(&arv);

        printf("\n");
    }

    fclose(csv);
    printf("  Resultados gravados em resultados/resultados.csv\n");
}

/* ================================================================== */
/* main                                                                */
/* ================================================================== */
int main(void) {
    /* Declaração e inicialização de todas as estruturas de dados */
    Fila       fila;
    Lista      lista;
    Arvore     arvore;
    TabelaHash hash;

    fila_inicializar(&fila);
    lista_inicializar(&lista);
    arvore_inicializar(&arvore);
    hash_inicializar(&hash);

    /* Contador global de IDs (começa em 1 e é incrementado a cada cadastro) */
    int proximo_id = 1;

    int opcao;

    printf("\n  Bem-vindo ao Sistema de Controle de Atendimentos!\n");

    do {
        exibir_menu();
        opcao = ler_inteiro();

        switch (opcao) {

            /* ---- 1: Cadastrar atendimento ---- */
            case 1:
                menu_cadastrar(&fila, &lista, &arvore, &hash, &proximo_id);
                break;

            /* ---- 2: Atender próximo da fila ---- */
            case 2:
                menu_atender_proximo(&fila, &lista);
                break;

            /* ---- 3: Listar fila de espera ---- */
            case 3:
                printf("\n  --- Fila de Espera ---\n");
                fila_imprimir(&fila);
                break;

            /* ---- 4: Listar histórico completo ---- */
            case 4:
                printf("\n  --- Historico Completo de Atendimentos ---\n");
                lista_imprimir(&lista);
                break;

            /* ---- 5: Busca sequencial por nome ---- */
            case 5: {
                char nome[TAM_CLIENTE];
                printf("\n  --- Busca por Nome do Cliente ---\n");
                printf("  Nome (ou parte do nome): ");
                ler_string(nome, TAM_CLIENTE);
                lista_buscar_por_cliente(&lista, nome);
                break;
            }

            /* ---- 6: Busca por ID na Árvore BST ---- */
            case 6: {
                printf("\n  --- Busca por ID (Arvore BST) ---\n");
                printf("  ID do atendimento: ");
                int id = ler_inteiro();
                const Atendimento *encontrado = arvore_buscar(&arvore, id);
                if (encontrado) {
                    const char *prio_str;
                    switch (encontrado->prioridade) {
                        case 3:  prio_str = "Alta";  break;
                        case 2:  prio_str = "Media"; break;
                        default: prio_str = "Baixa"; break;
                    }
                    printf("  [Encontrado]\n");
                    printf("  ID:         %d\n",    encontrado->id);
                    printf("  Cliente:    %s\n",    encontrado->cliente);
                    printf("  Categoria:  %s\n",    encontrado->categoria);
                    printf("  Prioridade: %s (%d)\n", prio_str, encontrado->prioridade);
                    printf("  Tempo:      %d min\n", encontrado->tempo_estimado);
                    printf("  Status:     %s\n",    encontrado->status);
                } else {
                    printf("  Atendimento com ID %d nao encontrado.\n", id);
                }
                break;
            }

            /* ---- 7: Busca por categoria na Hash ---- */
            case 7: {
                char categoria[TAM_CATEGORIA];
                printf("\n  --- Busca por Categoria (Tabela Hash) ---\n");
                printf("  Categoria: ");
                ler_string(categoria, TAM_CATEGORIA);
                hash_buscar_por_categoria(&hash, categoria);
                break;
            }

            /* ---- 8: Ordenar histórico por prioridade ---- */
            case 8:
                printf("\n  --- Ordenando Historico por Prioridade ---\n");
                ordenar_lista(&lista);
                break;

            /* ---- 9: Exibir estatísticas ---- */
            case 9:
                menu_exibir_estatisticas(&lista, &fila);
                break;

            /* ---- 10: Experimentos de comparação de buscas ---- */
            case 10:
                executar_experimentos();
                break;

            /* ---- 0: Sair ---- */
            case 0:
                printf("\n  Liberando memoria e encerrando o sistema...\n");
                /* Libera todas as estruturas alocadas dinamicamente */
                fila_destruir(&fila);
                lista_destruir(&lista);
                arvore_destruir(&arvore);
                hash_destruir(&hash);
                printf("  Memoria liberada. Ate logo!\n\n");
                break;

            default:
                printf("\n  [Erro] Opcao invalida. Digite um numero de 0 a 10.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
