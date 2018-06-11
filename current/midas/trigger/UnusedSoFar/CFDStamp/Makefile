CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOT5SYS)/bin/root-config --cflags)
ROOT5 =  $(shell $(ROOT5SYS)/bin/root-config --glibs)
INC = -I$(ROOT5INC)\
      -I$(TIPsort)/current/utils\
      -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/midas/decoder\
      -I$(TIPsort)/current/midas/map\
      -I$(TIPsort)/current/midas/state-machine
BIN=$(TIPsort)/bin/midas
SORT= sort.o\
       $(TIPsort)/current/midas/state-machine/sort_but_not_assemble.a\
       $(TIPsort)/current/SFU-format/SFU-format.a\
       $(TIPsort)/current/midas/decoder/decoder.a\
       $(TIPsort)/current/midas/map/map.a\
       $(TIPsort)/current/utils/utils.a
sort:$(SORT)
	$(CC) -o $(BIN)/trig_CFDStamps $(SORT) $(CFLAGS) $(ROOT5) -lm -ldl

sort.o:sort.c 
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~