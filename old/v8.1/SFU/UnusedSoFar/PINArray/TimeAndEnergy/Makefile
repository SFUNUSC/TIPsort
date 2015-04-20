all:
	echo Making PINArray_TECal.
	(cd TECal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_TECAL.

	echo Making PINArray_TECalSum.
	(cd TECalSum r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_TECALSUM.

	echo Making PINArray_ECalTFitRaw.
	(cd ECalTFitRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_ECalTFitRaw.

clean:
	(cd TECal       && make -is clean && cd ..)
	(cd TECalSum    && make -is clean && cd ..)
	(cd ECalTFitRaw && make -is clean && cd ..)
	rm -f *~