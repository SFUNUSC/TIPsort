all:
	echo making rawSpectra_TigressCoreE
	(cd TigressCoreE r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_TIGRESSCOREE

	echo making rawSpectra_TigressSegE
	(cd TigressSegE r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_TIGRESSSEGE

	echo making rawSpectra_PINArrayE
	(cd PINArrayE r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_PINArrayE

	echo making rawSpectra_GriffinSegE
	(cd GriffinSegE r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_GRIFFINSEGE


clean:
	(cd TigressCoreE && make -is clean && cd ..)
	(cd TigressSegE && make -is clean && cd ..)
	(cd GriffinSegE && make -is clean && cd ..)
	(cd PINArrayE && make -is clean && cd ..)

	rm *~