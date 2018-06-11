CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOT5SYS)/bin/root-config --cflags)
ROOT5 =  $(shell $(ROOT5SYS)/bin/root-config --glibs)
INC = -I$(ROOT5INC)\
      -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/utils\
      -I$(TIPsort)/current/midas/decoder\
      -I$(TIPsort)/current/calibrator

BIN=$(TIPsort)/bin/sfu

SORT= sort.o\
      $(TIPsort)/current/SFU-format/SFU-format.a\
      $(TIPsort)/current/calibrator/calibrator.a\
      $(TIPsort)/current/utils/utils.a


sort:$(SORT)
	$(CC) -o $(BIN)/TigressCsIArray_ECalABSum_CsIEGate_CsIPIDr $(SORT) $(CFLAGS) $(ROOT5) -lm -ldl

sort.o:sort.c sort.h
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~