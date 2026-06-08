# SA2 - Análise Visual e Comparativa de Buscas e Árvores

Projeto em C para comparação entre métodos de busca e árvore binária de busca. O sistema gera vetores de inteiros ordenados e embaralhados, executa busca sequencial, busca binária e busca em árvore, mede tempo com `clock()` e grava os resultados em `resultados/resultados.csv`.

## Estrutura

- `src/` contém todo o código-fonte em módulos separados.
- `resultados/resultados.csv` armazena os dados dos experimentos.
- `Makefile` compila o projeto.

## Compilação

```bash
make
```

## Execução

```bash
./sistema_atendimentos
```

No menu, escolha a opção 10 para executar automaticamente os experimentos.

## Experimentos executados

- 100 elementos ordenados
- 100 elementos embaralhados
- 1.000.000 elementos ordenados
- 1.000.000 elementos embaralhados

Para cada cenário, o programa testa buscas por valor próximo do início, do meio, do fim e inexistente, registrando comparações, tempo e altura da árvore.

## Observação

O caso com 1.000.000 de elementos ordenados torna a ABB impraticável para construção na forma atual, então essa limitação é registrada no programa e no CSV.
