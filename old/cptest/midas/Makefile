all:
	echo Making decoder.
	(cd decoder && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: DECODER.

	echo Making state-machine.
	(cd state-machine && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: STATE-MACHINE.

	echo Making map.
	(cd map && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: MAP.

	echo Making midas2sfu.
	(cd midas2sfu && make -is clean && make && make -is clean && cd ..)
	echo DONE: MIDAS2SFU.

	echo Making rawSpectra.
	(cd rawSpectra && make -is clean && make && make -is clean && cd ..)
	echo DONE: rawSPECTRA.

clean:
	(cd decoder       && make -is clean && cd ..)
	(cd state-machine && make -is clean && cd ..)
	(cd map           && make -is clean && cd ..)
	(cd midas2sfu     && make -is clean && cd ..)
	(cd rawSpectra    && make -is clean && cd ..)
	rm -f *~

very-clean:
	echo Making very-clean all directories and bin.
	(cd decoder && make -is very-clean && cd ..)
	(cd state-machine && make -is very-clean && cd ..)
	(cd map && make -is very-clean && cd ..)
	rm -f *~
	echo DONE: Making very-clean.

