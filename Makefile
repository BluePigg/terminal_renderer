CC = gcc
TARGET = flappy
ISMODULE = 2Drender.c Square.c Block.c Text.c
SRCS = flappy.c $(addprefix modules/,$(ISMODULE))
HEADERS = $(wildcard modules/*.h)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) -o $(TARGET) $(SRCS) -lm
