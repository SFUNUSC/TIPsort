all:
	echo Making Griffin_CsIArray_TTRaw.
	(cd TTRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_CSIARRAY_TTRAW.

	echo Making Griffin_CsIArray_TTCal.
	(cd TTCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_CSIARRAY_TTCAL.

clean:
	(cd TTRaw    && make -is clean && cd ..)
	(cd TTCal    && make -is clean && cd ..)
	rm -f *~