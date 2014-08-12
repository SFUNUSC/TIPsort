all:
	echo Making Griffin_CsIArray_TTECalLim.
	(cd TTECalLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_CSIARRAY_TTECALLIM.

clean:
	(cd TTECalLim  && make -is clean && cd ..)
	rm -f *~