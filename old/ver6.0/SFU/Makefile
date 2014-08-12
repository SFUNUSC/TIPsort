all:
	echo making decoder
	(cd decoder && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: DECODER

	echo making check
	(cd check && make -is clean && make  && make -is clean && cd ..)
	echo DONE:CHECK

	echo making display
	(cd display && make -is clean && make  && make -is clean && cd ..)
	echo DONE:DISPLAY

	echo making separate
	(cd  separate && make -is clean && make  && make -is clean && cd ..)
	echo DONE:SEPARATE

	echo making PINARRAY
	(cd PINARRAY && make -is clean && make  && make -is clean && cd ..)
	echo DONE:PINARRAY

	echo making CSIARRAY
	(cd CSIARRAY && make -is clean && make  && make -is clean && cd ..)
	echo DONE:CSIARRAY

	echo making Tigress
	(cd Tigress && make -is clean && make  && make -is clean && cd ..)
	echo DONE:Tigress

	echo making Griffin
	(cd Griffin && make -is clean && make  && make -is clean && cd ..)
	echo DONE:Griffin

	echo making Tigress_PINARRAY
	(cd Tigress_PINARRAY && make -is clean && make  && make -is clean && cd ..)
	echo DONE:Tigress_PINARRAY

clean:
	(cd decoder && make -is clean && cd ..)
	(cd check && make -is clean && cd ..)
	(cd display && make -is clean && cd ..)
	(cd separate && make -is clean && cd ..)
	(cd Tigress && make -is clean && cd ..)
	(cd Griffin && make -is clean && cd ..)
	(cd PINARRAY && make -is clean && cd ..)
	(cd CSIARRAY && make -is clean && cd ..)
	(cd Tigress_PINARRAY && make -is clean && cd ..)
	rm *~
