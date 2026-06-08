# Makefile — Sistema de Controle de Atendimentos
# ================================================
# Uso:
#   make          -> compila o projeto
#   make clean    -> remove os arquivos compilados
#   make run      -> compila e executa
#
# Estrutura:
#   src/          -> todos os arquivos .c e .h
#   resultados/   -> CSV gerado automaticamente ao executar opção 10

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99
SRC     = src
TARGET  = sistema_atendimentos

SRCS = $(SRC)/main.c $(SRC)/fila.c $(SRC)/lista.c $(SRC)/ordenacao.c \
       $(SRC)/arvore.c $(SRC)/hash.c $(SRC)/buscas.c

OBJS = $(SRCS:.c=.o)

# Regra padrão: compila o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Compilacao concluida: $(TARGET)"

# Regra genérica: compila cada .c em .o incluindo o diretório src/
$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

# Dependências explícitas
$(SRC)/main.o:      $(SRC)/main.c      $(SRC)/atendimento.h $(SRC)/fila.h \
                    $(SRC)/lista.h $(SRC)/ordenacao.h $(SRC)/arvore.h \
                    $(SRC)/hash.h $(SRC)/buscas.h

$(SRC)/fila.o:      $(SRC)/fila.c      $(SRC)/fila.h      $(SRC)/atendimento.h
$(SRC)/lista.o:     $(SRC)/lista.c     $(SRC)/lista.h     $(SRC)/atendimento.h
$(SRC)/ordenacao.o: $(SRC)/ordenacao.c $(SRC)/ordenacao.h $(SRC)/lista.h
$(SRC)/arvore.o:    $(SRC)/arvore.c    $(SRC)/arvore.h    $(SRC)/atendimento.h
$(SRC)/hash.o:      $(SRC)/hash.c      $(SRC)/hash.h      $(SRC)/atendimento.h
$(SRC)/buscas.o:    $(SRC)/buscas.c    $(SRC)/buscas.h

# Compila e executa
run: $(TARGET)
	./$(TARGET)

# Remove arquivos gerados pela compilação
clean:
	del /Q $(SRC)\*.o $(TARGET).exe 2>nul || rm -f $(SRC)/*.o $(TARGET)

.PHONY: run clean

