CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOT5SYS)/bin/root-config --cflags)
ROOT5 =  $(shell $(ROOT5SYS)/bin/root-config --glibs)
INC = -I$(ROOT5INC)\
      -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/utils\
      -I$(TIPsort)/current/midas/decoder\
      -I$(TIPsort)/current/calibrator

BIN=$(TIPsort)/bin

SPECBS=SpecBS.o\
      $(TIPsort)/current/SFU-format/SFU-format.a\
      $(TIPsort)/current/calibrator/calibrator.a\
      $(TIPsort)/current/utils/utils.a

SpecBS:$(SPECBS)
	$(CC) -o $(BIN)/SpecBS $(SPECBS) $(CFLAGS) $(ROOT5) -lm -ldl

SpecBS.o:SpecBS.c SpecBS.h
	$(CC) -c -o SpecBS.o SpecBS.c $(CFLAGS) $(INC)

clean:
	rm -rf SpecBS *.o *~