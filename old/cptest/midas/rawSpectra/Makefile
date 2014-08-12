all:
	echo Making rawSpectra_GriffinE.
	(cd Griffin_ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWSPECTRA_GRIFFINE

clean:
	(cd Griffin_ERaw && make -is clean && cd ..)
	rm -f *~