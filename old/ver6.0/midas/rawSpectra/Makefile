all:
	echo making T1090
	(cd T10T90 r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_T10T90

	echo making rawSpectra_GriffinSegE
	(cd GriffinSegE r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_GRIFFINSEGE


clean:
	(cd TigressCoreE && make -is clean && cd ..)
	(cd TigressSegE && make -is clean && cd ..)
	(cd GriffinSegE && make -is clean && cd ..)
	(cd PINArrayE && make -is clean && cd ..)

	rm *~