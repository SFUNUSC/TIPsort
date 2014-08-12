all:
	echo making display_RawEvent
	(cd RawEvent r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_RawEvent

	echo making display_Tigress
	(cd Tigress r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_TIGRESS

	echo making display_Griffin
	(cd Griffin r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_GRIFFIN

	echo making display_PINARRAY
	(cd PINARRAY r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_PINARRAY

	echo making display_CSIARRAY
	(cd CSIARRAY r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_CSIARRAY

	echo making display_RF
	(cd RF r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_RF

	echo making display_PINBD
	(cd PINBD r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_PINBD

	echo making display_S3
	(cd S3 r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: DISPLAY_S3

clean:
	(cd RawEvent && make -is clean && cd ..)
	(cd Tigress && make -is clean && cd ..)
	(cd Griffin && make -is clean && cd ..)
	(cd PINARRAY && make -is clean && cd ..)
	(cd CSIARRAY && make -is clean && cd ..)
	(cd RF && make -is clean && cd ..)
	(cd PINBD && make -is clean && cd ..)
	(cd S3 && make -is clean && cd ..)
	rm *~