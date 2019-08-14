TARGET = main
LIBS = `pkg-config --cflags --libs libgit2`
CC = gcc
CFLAGS = -g -Wall
ODIR = ./build
SDIR = ./src

.PHONY: default all clean

default: $(ODIR)/$(TARGET)
all: default

OBJECTS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(wildcard $(SDIR)/*.c))
HEADERS = $(wildcard $(SDIR)/*.h)

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(ODIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(ODIR)/*
