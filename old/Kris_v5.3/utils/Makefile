CC = g++
CFLAGS   = -O3 -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
BIN = $(sort_TRIUMF_dir)/current/utils
INC = -I$(sort_TRIUMF_dir)/current/utils\
      -I$(sort_TRIUMF_dir)/current/midas/decoder\
      -I$(ROOTINC)

UTILS=read_master.o\
      lin_eq_solver.o\
      get_sin_par.o\
      waveform_analyzer.o


utils:$(UTILS) addspn
	$ ar rcs $(BIN)/utils.a $(UTILS)
	mv addspn $(sort_TRIUMF_dir)/bin

read_master.o:read_master.c read_master.h
	$(CC) -c -o read_master.o $(CFLAGS) read_master.c $(INC)

lin_eq_solver.o:lin_eq_solver.c lin_eq_solver.h
	$(CC) -c -o lin_eq_solver.o $(CFLAGS) lin_eq_solver.c $(INC)

get_sin_par.o:get_sin_par.c get_sin_par.h
	$(CC) -c -o get_sin_par.o $(CFLAGS) get_sin_par.c $(INC)

waveform_analyzer.o:waveform_analyzer.c waveform_analyzer.h
	$(CC) -c -o waveform_analyzer.o $(CFLAGS) waveform_analyzer.c $(INC) $(ROOT)

addspn:
	$(CC) -o addspn $(CFLAGS) addspn.c $(INC)

addmca:
	$(CC) -o addmca $(CFLAGS) addmca.c $(INC)
clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort addspn addmca