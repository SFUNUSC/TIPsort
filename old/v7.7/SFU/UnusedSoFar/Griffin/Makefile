all:
	echo Making Energy.
	(cd Energy r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Energy.

	echo Making Time.
	(cd Time r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Time.

	echo Making TimeAndEnergy.
	(cd TimeAndEnergy r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TimeAndEnergy.

clean:
	(cd Energy        && make -is clean && cd ..)
	(cd Time          && make -is clean && cd ..)
	(cd TimeAndEnergy && make -is clean && cd ..)
	rm -f *~