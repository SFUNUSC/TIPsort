all:

#	echo Setting up directories.
#	(mkdir ../bin && mkdir ../bin/midas && mkdir ../bin/sfu)

	echo Making utils.
	(cd utils && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: UTILS.

	echo Making SFU-format.
	(cd SFU-format && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: SFU-FORMAT.

	echo Making calibrator.
	(cd calibrator && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: CALIBRATOR.

	echo Making midas.
	(cd midas && make -is clean && make && make -is clean && cd ..)
	echo DONE: MIDAS.

	echo Making SFU.
	(cd SFU && make -is clean && make && make -is clean && cd ..)
	echo DONE: SFU.

	echo Making GateMaker.
	(cd GateMaker && make -is clean && make && make -is clean && cd ..)
	echo DONE: GATEMAKER.

	echo Making MatrixBS.
	(cd MatrixBS  && make -is clean && make && make -is clean && cd ..)
	echo DONE: MatrixBS.

	echo Making SpecBS.
	(cd SpecBS  && make -is clean && make && make -is clean && cd ..)
	echo DONE: SpecBS.

clean:
	echo Making clean all directories and bin.
	rm -rf ../bin/*
	rmdir ../bin
	(cd utils      && make -is clean && cd ..)
	(cd SFU-format && make -is clean && cd ..)
	(cd calibrator && make -is clean && cd ..)
	(cd SFU        && make -is clean && cd ..)
	(cd midas      && make -is clean && cd ..)
	(cd GateMaker  && make -is clean && cd ..)
	(cd MatrixBS   && make -is clean && cd ..)
	(cd SpecBS   && make -is clean && cd ..)
	rm -f *~
	echo DONE: Making clean.

very-clean:
	echo Making very-clean all directories and bin.
	(cd utils      && make -is very-clean && cd ..)
	(cd SFU-format && make -is very-clean && cd ..)
	(cd calibrator && make -is very-clean && cd ..)
	(cd midas      && make -is very-clean && cd ..)
	rm -f *~
	echo DONE: Making very-clean.
