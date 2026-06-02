CC = gcc
TARGET = 2Dtest
ISMODULE = 2Drender.c Square.c Block.c Text.c
SRCS = 2Dtest.c $(addprefix modules/,$(ISMODULE))
HEADERS = $(wildcard modules/*.h)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) -o $(TARGET) $(SRCS) -lm
