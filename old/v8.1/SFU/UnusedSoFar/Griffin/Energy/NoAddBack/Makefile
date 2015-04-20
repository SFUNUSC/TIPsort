all:
	echo Making Griffin_ERaw.
	(cd ERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ERaw.

	echo Making Griffin_ECal.
	(cd ECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECal.

	echo Making Griffin_ECalArray.
	(cd ECalArray r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECalArray.

	echo Making Griffin_ECalSum.
	(cd ECalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECalSum.

	echo Making Griffin_2DECal.
	(cd 2DECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_2DECal.

clean:
	(cd ERaw      && make -is clean && cd ..)
	(cd ECal      && make -is clean && cd ..)
	(cd ECalArray && make -is clean && cd ..)
	(cd ECalSum   && make -is clean && cd ..)
	(cd 2DECal    && make -is clean && cd ..)
	rm -f *~