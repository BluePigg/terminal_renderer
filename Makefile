CC = gcc
TARGET = 2Dtest
SRCS = 2Dtest.c 2Drender.c
HEADERS = t_renderer.h

$(TARGET): $(SRCS) $(HEADERS)
	gcc -o $(TARGET) $(SRCS) -lm
