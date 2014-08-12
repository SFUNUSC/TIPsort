all:
	echo Making utils.
	(cd utils && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: UTILS.

	echo Making SFU-format.
	(cd SFU-format && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: SFU-FORMAT.

	echo Making calibrator.
	(cd calibrator && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: CALIBRATOR.

	echo making midas.
	(cd midas && make -is clean && make && make -is clean && cd ..)
	echo DONE: MIDAS.

	echo making SFU.
	(cd SFU && make -is clean && make && make -is clean && cd ..)
	echo DONE: SFU.

clean:
	echo Making clean all directories and bin.
	rm -rf ../bin/midas/*
	rm -rf ../bin/sfu/*
	(cd utils      && make -is clean && cd ..)
	(cd SFU-format && make -is clean && cd ..)
	(cd calibrator && make -is clean && cd ..)
	(cd SFU        && make -is clean && cd ..)
	(cd midas      && make -is clean && cd ..)
	rm -f *~
	echo DONE: Making clean.

very-clean:
	echo Making very-clean all directories and bin.
	(cd utils      && make -is very-clean && cd ..)
	(cd SFU-format && make -is very-clean && cd ..)
	(cd calibrator && make -is very-clean && cd ..)
	rm -f *~
	echo DONE: Making very-clean.