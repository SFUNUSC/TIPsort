CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
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
	$(CC) -o $(BIN)/view_channel $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c 
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~