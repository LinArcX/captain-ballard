TARGET = captain_ballard
LIBS = `pkg-config --cflags --libs libgit2 glfw3 gl glu`
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
