CC=g++
INCLUDES=-I../
SOURCES=sqlite_db.cc db_factory.cc
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJECTS)
