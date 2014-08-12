CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/SFU/decoder\
      -I$(TIPsort)/current/utils

BIN=$(TIPsort)/bin/sfu

SORT= sort.o\
      $(TIPsort)/current/SFU/decoder/decoder.a

sort:$(SORT)
	$(CC) -o $(BIN)/CSIARRAY_EFitRaw $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c sort.h
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~