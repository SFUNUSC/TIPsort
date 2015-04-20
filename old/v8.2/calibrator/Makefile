CC = g++
CFLAGS   = -g -O3 -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)

ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
BIN = $(TIPsort)/current/calibrator
INC = -I$(TIPsort)/current/SFU-format\
      -I$(TIPsort)/current/midas/decoder\
     -I$(TIPsort)/current/utils\
      -I$(ROOTINC)

CALIBRATOR=SFU-cal.o\
	   SFU-cal-S3.o\
	   SFU-cal-TIGRESS.o\
	   SFU-cal-GRIFFIN.o\
	   SFU-cal-PINARRAY.o\
	   SFU-cal-CSIARRAY.o

calibrator:$(CALIBRATOR)
	$ ar rcs $(BIN)/calibrator.a $(CALIBRATOR)

SFU-cal.o:SFU-cal.c SFU-cal.h
	$(CC) -c -o SFU-cal.o $(CFLAGS) SFU-cal.c $(INC)

SFU-cal-PINARRAY.o:SFU-cal-PINARRAY.c SFU-cal-PINARRAY.h
	$(CC) -c -o SFU-cal-PINARRAY.o $(CFLAGS) SFU-cal-PINARRAY.c $(INC)

SFU-cal-CSIARRAY.o:SFU-cal-CSIARRAY.c SFU-cal-CSIARRAY.h
	$(CC) -c -o SFU-cal-CSIARRAY.o $(CFLAGS) SFU-cal-CSIARRAY.c $(INC)

SFU-cal-TIGRESS.o:SFU-cal-TIGRESS.c SFU-cal-TIGRESS.h
	$(CC) -c -o SFU-cal-TIGRESS.o $(CFLAGS) SFU-cal-TIGRESS.c $(INC)

SFU-cal-GRIFFIN.o:SFU-cal-GRIFFIN.c SFU-cal-GRIFFIN.h
	$(CC) -c -o SFU-cal-GRIFFIN.o $(CFLAGS) SFU-cal-GRIFFIN.c $(INC)

SFU-cal-S3.o:SFU-cal-S3.c SFU-cal-S3.h
	$(CC) -c -o SFU-cal-S3.o $(CFLAGS) SFU-cal-S3.c $(INC)

clean:
	rm -rf *.o 

very-clean:
	rm -rf *.a *.o *.*~ *~ sort