CC = gcc
TARGET = snake
ISMODULE = 2Drender.c Square.c Block.c
SRCS = snake.c $(addprefix modules/,$(ISMODULE))
HEADERS = $(wildcard modules/*.h)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) -o $(TARGET) $(SRCS) -lm
