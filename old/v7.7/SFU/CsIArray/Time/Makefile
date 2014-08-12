all:
	echo Making TCal.
	(cd TCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TCAL.

	echo Making TvsRFPhase.
	(cd TvsRFPhase r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TvsRFPHASE.

	echo Making TTCal.
	(cd TTCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TTCAL.

	echo Making TFitCFD.
	(cd TFitCFD r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TFITCFD.

clean:
	(cd TCal       && make -is clean && cd ..)
	(cd TvsRFPhase && make -is clean && cd ..)
	(cd TTCal      && make -is clean && cd ..)
	(cd TFitCFD    && make -is clean && cd ..)

	rm -f *~