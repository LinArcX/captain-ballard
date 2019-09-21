TARGET = captain_ballard
CC = gcc
LIBS = `pkg-config --libs libgit2 gtk+-3.0 glfw3 gl glu sqlite3`
CFLAGS = -g -Wall `pkg-config --cflags gtk+-3.0`
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
