CC = g++
CFLAGS   = -O3 -Wall -fPIC
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
BIN = $(sort_TRIUMF_dir)/current/calibrator
INC = -I$(sort_TRIUMF_dir)/current/SFU-format\
      -I$(sort_TRIUMF_dir)/current/utils\
      -I$(sort_TRIUMF_dir)/current/midas/decoder\
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