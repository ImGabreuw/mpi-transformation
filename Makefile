BIN = main
SRC = main.c

CFLAGS = -Wall -Wextra

NP ?= 5

.PHONY: all compile run clean

all: clean compile run

compile: $(SRC)
	@mpicc $(CFLAGS) -o $(BIN) $(SRC)

run: compile
	@mpirun -np $(NP) ./$(BIN)

clean:
	@rm -f $(BIN)
