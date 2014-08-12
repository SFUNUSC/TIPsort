all:
	echo Making rawSpectra_GriffinE.
	(cd Griffin_ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_GRIFFINE.

	echo Making rawSpectra_PINArrayE.
	(cd PINArray_ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_PINARRAYE.

	echo Making rawSpectra_CsIArrayE.
	(cd CsIArray_ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_CSIARRAYE.

	echo Making rawSpectra_TigressCore_ERaw.
	(cd TigressCore_ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_TIGRESSCOREE.

	echo Making rawSpectra_TigressCore_TRaw.
	(cd TigressCore_TRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_TIGRESSCORETIME.

clean:
	(cd Griffin_ERaw  && make -is clean && cd ..)
	(cd PINArray_ERaw && make -is clean && cd ..)
	(cd CsIArray_ERaw && make -is clean && cd ..)
	(cd TigressCore_ERaw && make -is clean && cd ..)
	(cd TigressCore_TRaw && make -is clean && cd ..)

	rm -f *~