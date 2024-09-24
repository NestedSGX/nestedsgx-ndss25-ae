CC=g++
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

%.o: %.cc
	$(CC) $(CFLAGS) -I../ -c $< -o $@

clean:
	$(RM) $(OBJECTS)
