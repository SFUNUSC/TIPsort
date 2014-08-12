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

clean:
	(cd ECalTLim    && make -is clean && cd ..)
	(cd ECalSumTLim && make -is clean && cd ..)
	(cd TECal       && make -is clean && cd ..)
	(cd TECalLim    && make -is clean && cd ..)
	rm -f *~