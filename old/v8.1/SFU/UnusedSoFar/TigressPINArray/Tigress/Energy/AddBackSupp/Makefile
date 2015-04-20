all:
	echo Making Tigress_ECalABSupp.
	(cd ECalABSupp r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSupp.

	echo Making Tigress_ECalABSuppSum.
	(cd ECalABSuppSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppSum.

	echo Making Tigress_ECalABSuppRingSum.
	(cd ECalABSuppRingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppRingSum.

	echo Making Tigress_ECalABSuppRingTLimSum.
	(cd ECalABSuppRingTLimSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppRingTLimSum.

	echo Making Tigress_ECalABSuppLayerSum.
	(cd ECalABSuppLayerSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppLayerSum.

	echo Making Tigress_TCalABSuppLayerSum.
	(cd TCalABSuppLayerSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TCalABSuppLayerSum.

	echo Making Tigress_ECalABSuppLayerSum_PINArrayRings.
	(cd ECalABSuppLayerSum_PINArrayRings r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppLayerSum_PINArrayRings.

	echo Making Tigress_ECalABSuppLayerTLimSum_PINArrayRings.
	(cd ECalABSuppLayerTLimSum_PINArrayRings r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppLayerTLimSum_PINArrayRings.

clean:
	(cd ECalABSupp                              && make -is clean && cd ..)
	(cd ECalABSuppSum                           && make -is clean && cd ..)
	(cd ECalABSuppRingSum                       && make -is clean && cd ..)
	(cd ECalABSuppRingTLimSum                   && make -is clean && cd ..)
	(cd ECalABSuppLayerSum                      && make -is clean && cd ..)
	(cd TCalABSuppLayerSum                      && make -is clean && cd ..)
	(cd ECalABSuppLayerSum_PINArrayRings        && make -is clean && cd ..)
	(cd ECalABSuppLayerTLimSum_PINArrayRings    && make -is clean && cd ..)
	rm -f *~