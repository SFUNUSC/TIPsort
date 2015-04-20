all:
	echo Making Tigress_ECalAB.
	(cd ECalAB r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalAB.

	echo Making Tigress_ECalABSum.
	(cd ECalABSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSum.

	echo Making Tigress_ECalABRing.
	(cd ECalABRing r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABRing.

	echo Making Tigress_ECalABRingSum.
	(cd ECalABRingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABRingSum.

	#echo Making Tigress_ECalABLayerSum.
	#(cd ECalABLayerSum r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABLayerSum.

clean:
	(cd ECalAB         && make -is clean && cd ..)
	(cd ECalABSum      && make -is clean && cd ..)
	(cd ECalABRing     && make -is clean && cd ..)
	(cd ECalABRingSum  && make -is clean && cd ..)
	#(cd ECalABLayerSum && make -is clean && cd ..)
	rm -f *~	