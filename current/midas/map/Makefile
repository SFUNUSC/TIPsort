CC = g++
CFLAGS   = -O3 -Wall -fPIC

BIN = $(TIPsort)/current/midas/map
INC = -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/midas/decoder\
      -I$(TIPsort)/current/utils\
      -I$(ROOT5INC)
MAP=map.o

map:$(MAP)
	$ ar rcs $(BIN)/map.a $(MAP)


map.o:map.c map.h
	$(CC) -c -o map.o $(CFLAGS) map.c $(INC)

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort