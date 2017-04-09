TARGET = mvgame
LIBS = -ltermbox
CC = gcc
CFLAGS = -g -O0 -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = misc.o  error.o  level.o  main.o  player.o
HEADERS = misc.hh error.hh level.hh main.hh player.hh

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
