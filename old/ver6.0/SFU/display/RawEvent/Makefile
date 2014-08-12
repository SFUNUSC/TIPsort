CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(sort_TRIUMF_dir)/current/SFU-format\
      -I$(sort_TRIUMF_dir)/current/SFU/decoder\
      -I$(sort_TRIUMF_dir)/current/utils

BIN=$(sort_TRIUMF_dir)/bin/sfu

SORT= sort.o\
      $(sort_TRIUMF_dir)/current/SFU-format/SFU-format.a\
      $(sort_TRIUMF_dir)/current/SFU/decoder/decoder.a

sort:$(SORT)
	$(CC) -o $(BIN)/display_RawEvent $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c sort.h
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~