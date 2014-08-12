all:
	echo Making CsIArray_TFitRaw.
	(cd TFitRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CsIArray_TFITRAW.

	echo Making PID.
	(cd PID r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PID.

	echo Making GoodnessOfFit.
	(cd GoodnessOfFit r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GoodnessOfFit.
clean:
	(cd TFitRaw       && make -is clean && cd ..)
	(cd PID           && make -is clean && cd ..)
	(cd GoodnessOfFit && make -is clean && cd ..)
	rm -f *~