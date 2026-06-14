CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lm

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Encontrar todos os .c em src/ e src/questions/ (excluindo testes)
ALL_SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/questions/*.c)
SRCS = $(filter-out $(SRC_DIR)/test_%.c, $(ALL_SRCS))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Executável
TARGET = trabalho3

.PHONY: all clean run

MAIN_DEPS = $(OBJ_DIR)/main.o $(OBJ_DIR)/arquivo.o $(OBJ_DIR)/indice_bst.o $(OBJ_DIR)/tabela_hash.o $(OBJ_DIR)/indice_intervalo_avl.o $(OBJ_DIR)/lista.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/produto.o $(OBJ_DIR)/question0.o $(OBJ_DIR)/question1.o $(OBJ_DIR)/question2.o

all: $(TARGET)

$(TARGET): $(MAIN_DEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/questions/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	mkdir -p data
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
