CC = g++
CFLAGS   = -O3 -Wall -fPIC

INC = -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/utils

BIN = $(TIPsort)/current/SFU/decoder

DECODER=SFU-decoder.o

decoder:$(DECODER)
	$ ar rcs $(BIN)/decoder.a $(DECODER)

SFU-decoder.o:SFU-decoder.c SFU-decoder.h
	$(CC) -c -o SFU-decoder.o $(CFLAGS) $(INC) SFU-decoder.c
clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort