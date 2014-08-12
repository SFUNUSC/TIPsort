CC = g++
CFLAGS   = -O3 -Wall -fPIC
BIN = $(TIPsort)/current/SFU-format
INC = -I$(TIPsort)/current/utils\
      -I$(TIPsort)/current/midas/decoder\
      -I$(ROOTINC)

SFU-FORMAT=SFU-format.o SFU-decoder.o SFU-encoder.o

SFU-format:$(SFU-FORMAT)
	$ ar rcs $(BIN)/SFU-format.a $(SFU-FORMAT)

SFU-format.o:SFU-format.c SFU-format.h SFU-common.h
	$(CC) -c -o SFU-format.o $(CFLAGS) SFU-format.c $(INC)

SFU-decoder.o:SFU-decoder.c SFU-decoder.h
	$(CC) -c -o SFU-decoder.o $(CFLAGS) $(INC) SFU-decoder.c

SFU-encoder.o:SFU-encoder.c SFU-encoder.h
	$(CC) -c -o SFU-encoder.o $(CFLAGS) $(INC) SFU-encoder.c

print_sizes: print_sizes.c SFU-format.h SFU-common.h
	$(CC) -o print_sizes $(CFLAGS) print_sizes.c $(INC)

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort