all:
	echo Making TFitRaw.
	(cd TFitRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TFITRAW.

	echo Making TRaw.
	(cd TRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TRAW.

	echo Making TCal.
	(cd TCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TCAL.

	echo Making TvsRFPhase.
	(cd TvsRFPhase r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TvsRFPHASE.

	echo Making TTRaw.
	(cd TTRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TTRAW.

clean:
	(cd TFitRaw    && make -is clean && cd ..)
	(cd TRaw       && make -is clean && cd ..)
	(cd TCal       && make -is clean && cd ..)
	(cd TvsRFPhase && make -is clean && cd ..)
	(cd TTRaw      && make -is clean && cd ..)
	rm -f *~