all:
	echo making Energy.
	(cd Energy r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY_ENERGY.

	echo making Time.
	(cd Time r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY_TIME.

	echo making TimeAndEnergy.
	(cd TimeAndEnergy r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY_TIMEANDENERGY.

clean:
	(cd Energy        && make -is clean && cd ..)
	(cd Time          && make -is clean && cd ..)
	(cd TimeAndEnergy && make -is clean && cd ..)
	rm -f *~