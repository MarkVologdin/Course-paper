CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Wno-unused-but-set-variable
LDFLAGS = -lm

TARGET = my_program
SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

memcheck: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)
	
run: $(TARGET)
	./$(TARGET)