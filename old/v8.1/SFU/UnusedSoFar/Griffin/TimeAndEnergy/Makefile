all:
	echo Making Griffin_ECalTLim.
	(cd ECalTLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECalTLim.

	echo Making Griffin_ECalSumTLim.
	(cd ECalSumTLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECalSumTLim.

	echo Making Griffin_TECal.
	(cd TECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_TECal.

	echo Making Griffin_TECalLim.
	(cd TECalLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_TECalLim.

	echo Making Griffin_TTECalLim.
	(cd TTECalLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_TTECalLim.

	echo Making Griffin_TTECalButEABLim.
	(cd TTECalButEABLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_TTECalButEABLim.

	echo Making Griffin_TTEABLim.
	(cd TTEABLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_TTEABLim.

	echo Making Griffin_ECalTFitRaw.
	(cd ECalTFitRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECalTFitRaw.

	echo Making Griffin_ECalTFitRawSum.
	(cd ECalTFitRawSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECalTFitRawSum.

clean:
	(cd ECalTLim        && make -is clean && cd ..)
	(cd ECalSumTLim     && make -is clean && cd ..)
	(cd TECal           && make -is clean && cd ..)
	(cd TECalLim        && make -is clean && cd ..)
	(cd TTECalLim       && make -is clean && cd ..)
	(cd TTECalButEABLim && make -is clean && cd ..)
	(cd TTEABLim        && make -is clean && cd ..)
	(cd ECalTFitRaw     && make -is clean && cd ..)
	(cd ECalTFitRawSum  && make -is clean && cd ..)
	rm -f *~