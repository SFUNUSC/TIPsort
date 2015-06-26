CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
	ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/midas/decoder\
      -I$(TIPsort)/current/calibrator\
      -I$(TIPsort)/current/utils

BIN=$(TIPsort)/bin/sfu

SORT= sort.o\
	$(TIPsort)/current/SFU-format/SFU-format.a\
	$(TIPsort)/current/calibrator/calibrator.a\
	$(TIPsort)/current/utils/utils.a

sort:$(SORT)
	$(CC) -o $(BIN)/CsIArray_TRaw $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c sort.h
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~