/*
 * atendimento.h
 * =============
 * Define o Tipo Abstrato de Dado (TAD) base do sistema: a struct Atendimento.
 * Este arquivo é incluído por todos os outros módulos que precisam manipular
 * dados de atendimento. O uso de include guard (#ifndef) evita inclusão dupla.
 */

#ifndef ATENDIMENTO_H
#define ATENDIMENTO_H

/* Constantes de tamanho para os campos de string */
#define TAM_CLIENTE   50
#define TAM_CATEGORIA 30
#define TAM_STATUS    15

/*
 * Struct Atendimento
 * ------------------
 * Representa um registro completo de atendimento ao cliente.
 * Todos os módulos do sistema manipulam ponteiros ou cópias desta struct.
 */
typedef struct {
    int  id;                       /* Identificador único do atendimento        */
    char cliente[TAM_CLIENTE];     /* Nome do cliente (até 49 chars + '\0')     */
    char categoria[TAM_CATEGORIA]; /* Categoria do atendimento (ex: "Suporte")  */
    int  prioridade;               /* 1 = baixa | 2 = média | 3 = alta          */
    int  tempo_estimado;           /* Duração estimada do atendimento (minutos) */
    char status[TAM_STATUS];       /* "aberto" | "atendido" | "cancelado"       */
} Atendimento;

#endif /* ATENDIMENTO_H */
