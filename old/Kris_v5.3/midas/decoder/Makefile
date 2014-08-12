CC = g++
CFLAGS   = -O3 -Wall -fPIC

BIN = $(sort_TRIUMF_dir)/current/midas/decoder

DECODER=midas-format.o tig-format.o

decoder:$(DECODER)
	$ ar rcs $(BIN)/decoder.a $(DECODER)

tig-format.o:tig-format.c tig-format.h
	$(CC) -c -o tig-format.o $(CFLAGS) tig-format.c

midas-format.o:midas-format.c midas-format.h
	$(CC) -c -o midas-format.o $(CFLAGS) midas-format.c

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort