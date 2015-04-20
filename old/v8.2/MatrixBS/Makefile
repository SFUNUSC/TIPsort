CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/utils\
      -I$(TIPsort)/current/midas/decoder\
      -I$(TIPsort)/current/calibrator

BIN=$(TIPsort)/bin

MATRIXBS=MatrixBS.o\
      $(TIPsort)/current/SFU-format/SFU-format.a\
      $(TIPsort)/current/calibrator/calibrator.a\
      $(TIPsort)/current/utils/utils.a

MatrixBS:$(MATRIXBS)
	$(CC) -o $(BIN)/MatrixBS $(MATRIXBS) $(CFLAGS) $(ROOT) -lm -ldl

MatrixBS.o:MatrixBS.c MatrixBS.h
	$(CC) -c -o MatrixBS.o MatrixBS.c $(CFLAGS) $(INC)

clean:
	rm -rf MatrixBS *.o *~