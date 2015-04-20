CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I/home1/tip/sortCode/current/SFU-format\
      -I/home1/tip/sortCode/current/SFU/decoder\
      -I/home1/tip/sortCode/current/utils

BIN=/home1/tip/sortCode/bin/sfu

SORT= sort.o\
      /home1/tip/sortCode/current/SFU/decoder/decoder.a

sort:$(SORT)
	$(CC) -o $(BIN)/Tigress_TGeBGO $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c sort.h
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~