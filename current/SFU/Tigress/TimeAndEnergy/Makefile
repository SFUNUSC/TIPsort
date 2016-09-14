all:
	echo making Tigress_TECal.
	(cd TECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TECal.

	echo making Tigress_ECalTFitRaw.
	(cd ECalTFitRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalTFitRaw.

clean:
	(cd TECal       && make -is clean && cd ..)
	(cd ECalTFitRaw && make -is clean && cd ..)
	rm -f *~