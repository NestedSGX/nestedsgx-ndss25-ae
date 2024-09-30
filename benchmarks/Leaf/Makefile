# 定义变量
CC = g++
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=)

all: $(OBJ)

%: %.cpp
	$(CC) -o $@ $<

bench: all
	@echo "Running executables:"
	@for file in $(OBJ); do \
		echo $$file; \
		./$$file; \
	done

clean:
	rm -f $(OBJ)
