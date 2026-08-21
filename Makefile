CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src/main.c src/db.c src/catalog.c src/table.c src/sql.c
BIN = minidb

.PHONY: all clean run

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f $(BIN)
	rm -rf data
	mkdir -p data

run: all
	./$(BIN)
