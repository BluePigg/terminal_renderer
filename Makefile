CC = gcc
TARGET = rng
ISMODULE = 2Drender.c Square.c Block.c Text.c
SRCS = rng.c $(addprefix modules/,$(ISMODULE))
HEADERS = $(wildcard modules/*.h)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) -o $(TARGET) $(SRCS) -lm
