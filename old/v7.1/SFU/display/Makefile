all:
	echo Making display_RawEvent.
	(cd RawEvent r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_RawEvent.

	echo Making display_Griffin.
	(cd Griffin r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_GRIFFIN.

	echo Making display_Tigress.
	(cd Tigress r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_TIGRESS.

	echo Making display_CsIArray.
	(cd CsIArray r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_CsIArray.

	echo Making display_PINBD.
	(cd PINBD r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_PINBD.

	echo Making display_PINArray.
	(cd PINArray r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_PINArray.

	echo Making display_S3.
	(cd S3 r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_S3.

	echo Making display_RF.
	(cd RF r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY_RF.

clean:
	(cd RawEvent && make -is clean && cd ..)
	(cd Griffin  && make -is clean && cd ..)
	(cd Tigress  && make -is clean && cd ..)
	(cd CsIArray && make -is clean && cd ..)
	(cd PINBD    && make -is clean && cd ..)
	(cd PINArray && make -is clean && cd ..)
	(cd S3       && make -is clean && cd ..)
	(cd RF       && make -is clean && cd ..)
	rm -f *~