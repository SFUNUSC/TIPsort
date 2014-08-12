CC = g++
CFLAGS   = -O3 -Wall -fPIC $(shell $(ROOTSYS)/bin/root-config --cflags)

ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
BIN = $(TIPsort)/current/SFU-format
INC = -I$(TIPsort)/current/utils\
      -I$(ROOTINC)

SFU-FORMAT=SFU-format.o SFU-decoder.o SFU-encoder.o

SFU-format:$(SFU-FORMAT) print_sizes
	$ ar rcs $(BIN)/SFU-format.a $(SFU-FORMAT)
	mv print_sizes $(TIPsort)/bin

SFU-format.o:SFU-format.c SFU-format.h SFU-common.h
	$(CC) -c -o SFU-format.o $(CFLAGS) SFU-format.c $(INC)

SFU-decoder.o:SFU-decoder.c SFU-decoder.h
	$(CC) -c -o SFU-decoder.o $(CFLAGS) $(INC) SFU-decoder.c

SFU-encoder.o:SFU-encoder.c SFU-encoder.h
	$(CC) -c -o SFU-encoder.o $(CFLAGS) $(INC) SFU-encoder.c

print_sizes: print_sizes.c SFU-format.h SFU-common.h
	$(CC) -o print_sizes $(CFLAGS) print_sizes.c $(INC) $(ROOT)

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort