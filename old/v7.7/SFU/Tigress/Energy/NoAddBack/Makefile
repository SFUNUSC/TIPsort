all:
	echo Making Tigress_ERaw.
	(cd ERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ERaw.

	echo Making Tigress_ESegRaw.
	(cd ESegRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ESegRaw.

	echo Making Tigress_ERawSum.
	(cd ERawSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ERawSum.

	echo Making Tigress_ECal.
	(cd ECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECal.

	echo Making Tigress_ECalSum.
	(cd ECalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalSum.

	echo Making Tigress_ECalRing.
	(cd ECalRing r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalRing.

	echo Making Tigress_ECalRingSum.
	(cd ECalRingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalRingSum.

	echo Making Tigress_ECalLayerSum.
	(cd ECalLayerSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalLayerSum.

clean:
	(cd ERaw         && make -is clean && cd ..)
	(cd ESegRaw      && make -is clean && cd ..)
	(cd ERawSum      && make -is clean && cd ..)
	(cd ECal         && make -is clean && cd ..)
	(cd ECalSum      && make -is clean && cd ..)
	(cd ECalRing     && make -is clean && cd ..)
	(cd ECalRingSum  && make -is clean && cd ..)
	(cd ECalLayerSum && make -is clean && cd ..)
	rm -f *~