# src/main.c src/run_shell.c src/execute_command.c src/run_process.c src/power.c src/fclean.c src/pclean.c src/time.c

CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/run_shell.c src/execute_command.c src/run_process.c src/power.c src/fclean.c src/pclean.c src/time.c
OBJ = $(SRC:.c=.o)
TARGET = eco-shell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

.PHONY: all clean
