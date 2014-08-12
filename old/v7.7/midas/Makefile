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

	echo Making project.
	(cd project && make -is clean && make && make -is clean && cd ..)
	echo DONE: PROJECt.

	echo Making view.
	(cd view && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW.

	echo Making trigger.
	(cd trigger && make -is clean && make && make -is clean && cd ..)
	echo DONE: TRIGGER.

	echo Making FFT.
	(cd FFT && make -is clean && make && make -is clean && cd ..)
	echo DONE: FFT.

	echo Making wfit.
	(cd wfit && make -is clean && make && make -is clean && cd ..)
	echo DONE: wfit.

clean:
	(cd decoder       && make -is clean && cd ..)
	(cd state-machine && make -is clean && cd ..)
	(cd map           && make -is clean && cd ..)
	(cd midas2sfu     && make -is clean && cd ..)
	(cd rawSpectra    && make -is clean && cd ..)
	(cd project       && make -is clean && cd ..)
	(cd view          && make -is clean && cd ..)
	(cd wfit          && make -is clean && cd ..)
	(cd trigger       && make -is clean && cd ..)
	rm -f *~

very-clean:
	echo Making very-clean all directories and bin.
	(cd decoder && make -is very-clean && cd ..)
	(cd state-machine && make -is very-clean && cd ..)
	(cd map && make -is very-clean && cd ..)
	rm -f *~
	echo DONE: Making very-clean.

