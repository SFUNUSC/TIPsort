CC = g++
CFLAGS   = -O -g -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(sort_TRIUMF_dir)/current/SFU-format\
      -I$(sort_TRIUMF_dir)/current/calibrator\
      -I$(sort_TRIUMF_dir)/current/utils

BIN=$(sort_TRIUMF_dir)/bin/sfu

SORT= sort.o\
$(sort_TRIUMF_dir)/current/SFU-format/SFU-format.a\
      $(sort_TRIUMF_dir)/current/calibrator/calibrator.a\
      $(sort_TRIUMF_dir)/current/utils/utils.a

sort:$(SORT)
	$(CC) -o $(BIN)/Griffin_ECal_CsIPIDTR $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c sort.h
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~