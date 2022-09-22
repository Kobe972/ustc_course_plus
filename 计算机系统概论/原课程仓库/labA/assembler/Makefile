CC=g++
CFLAGS=-I. -g
DEPS = assembler.h
OBJ = assembler.o main.o 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assembler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

all: assembler

.PHONY: clean

clean:
	rm -rf assembler
	rm *.o