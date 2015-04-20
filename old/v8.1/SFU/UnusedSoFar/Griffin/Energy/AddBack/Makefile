all:
	echo Making Griffin_ECalAB.
	(cd ECalAB r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalAB.

	echo Making Griffin_ECalABSum.
	(cd ECalABSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalABSum.

	echo Making Griffin_ECalABRing.
	(cd ECalABRing r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalABRing.

	echo Making Griffin_ECalABRingSum.
	(cd ECalABRingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalABRingSum.

	echo Making Griffin_ECalABLayerSum.
	(cd ECalABLayerSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalABLayerSum.

clean:
	(cd ECalAB         && make -is clean && cd ..)
	(cd ECalABSum      && make -is clean && cd ..)
	(cd ECalABRing     && make -is clean && cd ..)
	(cd ECalABRingSum  && make -is clean && cd ..)
	(cd ECalABLayerSum && make -is clean && cd ..)
	rm -f *~	