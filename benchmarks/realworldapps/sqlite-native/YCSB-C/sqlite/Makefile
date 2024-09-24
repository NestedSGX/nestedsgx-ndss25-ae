CC = gcc
SGX ?=

ifeq ($(SGX), 1)
	SRC := sqlite3_sgx
else
	SRC := sqlite3
endif

all: $(SRC).o

$(SRC).i: $(SRC).c
	$(CC) -DSQLITE_THREADSAFE=0 -E $< -o $@

$(SRC).o: $(SRC).i $(SRC).c
	$(CC) $(CFLAGS) -DSQLITE_THREADSAFE=0 $< -c -o $@

clean:
	$(RM) *.o *.i
