CC = g++
CFLAGS   = -O3 -Wall -fPIC

BIN = $(TIPsort)/current/SFU-format
INC = -I$(TIPsort)/current/utils
SFU-FORMAT=SFU-format.o

decoder:$(SFU-FORMAT)
	$ ar rcs $(BIN)/SFU-format.a $(SFU-FORMAT)

SFU-format.o:SFU-format.c SFU-format.h SFU-common.h
	$(CC) -c -o SFU-format.o $(CFLAGS) SFU-format.c $(INC)

print_sizes: print_sizes.c SFU-format.h SFU-common.h
	$(CC) -o print_sizes $(CFLAGS) print_sizes.c $(INC)

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort