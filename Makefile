TARGET = mvgame
CC = g++
LIBS = -ltermbox
CFLAGS = -Wno-write-strings -fpermissive -O1		# Without optimisations, a BUG IN THE COMPILER caused the counting of coins in Level :: Level() to break the checksum...

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = error.o level.o main.o misc.o player.o 
HEADERS = chars.h error.hh level.hh main.hh misc.hh mvgame.hh player.hh tiles.h

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
