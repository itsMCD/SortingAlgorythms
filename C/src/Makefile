CC = gcc
CFLAGS = -Wall -Wextra -g -DDEBUG

SRC = array_operations.c
OBJ = array_operations.o

TARGET = read_file

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
