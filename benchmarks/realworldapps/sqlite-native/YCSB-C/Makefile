CC=g++
CFLAGS ?=-m64 -O2 -Wall -Wextra -Winit-self -Wpointer-arith -Wreturn-type -Waddress -Wsequence-point -Wformat-security -Wmissing-include-dirs -Wfloat-equal -Wundef -Wshadow -Wcast-align -Wnon-virtual-dtor -std=c++11 -fvisibility=hidden -fpie -ffunction-sections -fdata-sections
LDFLAGS= -lpthread -ldl
SUBDIRS=core db sqlite
OBJ_TRTS=core/core_workload.o db/db_factory.o db/sqlite_db.o
LIB_TRTS=ycsbc_sgx.a
SGX ?=

ifeq ($(SGX), 1)
	OBJ_TRTS += sqlite/sqlite3_sgx.o
else
	OBJ_TRTS += sqlite/sqlite3.o
endif

export CFLAGS

all: $(SUBDIRS) $(LIB_TRTS)

$(SUBDIRS):
	$(MAKE) -C $@

$(LIB_TRTS): $(OBJ_TRTS)
	ar rvs $@ $^

native: ycsbc.cc $(SUBDIRS)
	$(CC) $(CFLAGS) -I. ycsbc.cc $(OBJ_TRTS) $(LDFLAGS) -o ycsbc

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	$(RM) *.o *.a

.PHONY: $(SUBDIRS) $(EXEC)
