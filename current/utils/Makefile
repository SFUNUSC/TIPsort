CC = g++
CFLAGS   = -g -O3 -Wall -fPIC  $(shell $(ROOT5SYS)/bin/root-config --cflags)

ROOT5 =  $(shell $(ROOT5SYS)/bin/root-config --glibs)
BIN = $(TIPsort)/current/utils
INC = -I$(TIPsort)/current/utils\
      -I$(TIPsort)/current/midas/decoder\
      -I$(ROOT5INC)

UTILS=read_master.o\
      lin_eq_solver.o\
      get_sin_par.o\
      waveform_analyzer.o

utils:$(UTILS) addspn
	$ ar rcs $(BIN)/utils.a $(UTILS)
	mv addspn $(TIPsort)/bin

read_master.o:read_master.c read_master.h
	$(CC) -c -o read_master.o $(CFLAGS) read_master.c -I$(INC)

lin_eq_solver.o:lin_eq_solver.c lin_eq_solver.h
	$(CC) -c -o lin_eq_solver.o $(CFLAGS) lin_eq_solver.c -I$(INC)

get_sin_par.o:get_sin_par.c get_sin_par.h
	$(CC) -c -o get_sin_par.o $(CFLAGS) get_sin_par.c -I$(INC)

waveform_analyzer.o:waveform_analyzer.c waveform_analyzer.h
	$(CC) -c -o waveform_analyzer.o $(CFLAGS) waveform_analyzer.c -I$(INC) $(ROOT5)

addspn:
	$(CC) -o addspn $(CFLAGS) addspn.c -I$(INC)
clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort addspn