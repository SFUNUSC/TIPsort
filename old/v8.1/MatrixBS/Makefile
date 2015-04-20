CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)

BIN=$(TIPsort)/bin

MATRIXBS=MatrixBS.o

MatrixBS:$(MATRIXBS)
	$(CC) -o $(BIN)/MatrixBS $(MATRIXBS) $(CFLAGS) $(ROOT) -lm -ldl

MatrixBS.o:MatrixBS.c MatrixBS.h
	$(CC) -c -o MatrixBS.o MatrixBS.c $(CFLAGS) $(INC)

clean:
	rm -rf MatrixBS *.o *~