CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(sort_TRIUMF_dir)/current/utils\
      -I$(sort_TRIUMF_dir)/current/SFU-format\
      -I$(sort_TRIUMF_dir)/current/midas/decoder\
      -I$(sort_TRIUMF_dir)/current/midas/map\
      -I$(sort_TRIUMF_dir)/current/midas/state-machine
BIN=$(sort_TRIUMF_dir)/bin/midas
SORT= sort.o\
       $(sort_TRIUMF_dir)/current/midas/state-machine/sort_and_assemble.a\
       $(sort_TRIUMF_dir)/current/SFU-format/SFU-format.a\
       $(sort_TRIUMF_dir)/current/midas/decoder/decoder.a\
       $(sort_TRIUMF_dir)/current/midas/map/map.a\
       $(sort_TRIUMF_dir)/current/utils/utils.a
sort:$(SORT)
	$(CC) -o $(BIN)/rawSpectra_TigressSegE $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c 
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~