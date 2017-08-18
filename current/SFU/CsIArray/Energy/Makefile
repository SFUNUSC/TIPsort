all:
	echo Making AngleBetweenHits.
	(cd AngleBetweenHits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ANGLEBETWEENHITS.	
	
	echo Making ERaw.
	(cd ERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ERAW.
	
	echo Making ERawSum.
	(cd ERawSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ERAWSUM.
	
	echo Making ECal.
	(cd ECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECAL.
	
	echo Making ECalSum.
	(cd ECalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALSUM.
	
	echo Making ECalRing.
	(cd ECalRing r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALRING.
	
	echo Making ECalTotalEventSum.
	(cd ECalTotalEventSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALTotalEventSUM.
	
	echo Making ECal_CsIPIDr.
	(cd ECal_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECAL_CSIPIDR.
	
	echo Making ECalSum_PIDr.
	(cd ECalSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALSUM_PIDR.
	
	echo Making ECalPosSum_PIDr.
	(cd ECalPosSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALPOSSUM_PIDR.
	
	echo Making ECalParticleSumRunSum_PIDr.
	(cd ECalParticleSumRunSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALPARTICLESUMRUNSUM_PIDR.
	
	echo Making EECalSum.
	(cd EECalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: EECALSUM.
	
	echo Making DeltaU.
	(cd DeltaU r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DELTAU.
	
	echo Making DeltaUSum.
	(cd DeltaUSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DELTAUSUM.
	
	echo Making DeltaURingSum.
	(cd DeltaURingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DELTAURINGSUM.
	
	echo Making DeltaUSameRingSum.
	(cd DeltaUSameRingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DELTAUSAMERINGSUM.

clean:
	(cd AngleBetweenHits 				&& make -is clean && cd ..)
	(cd ERaw 				&& make -is clean && cd ..)
	(cd ERawSum 				&& make -is clean && cd ..)
	(cd ECal 				&& make -is clean && cd ..)
	(cd ECalSum 				&& make -is clean && cd ..)
	(cd ECalRing 				&& make -is clean && cd ..)
	(cd ECalTotalEventSum 				&& make -is clean && cd ..)
	(cd ECal_CsIPIDr 			&& make -is clean && cd ..)
	(cd ECalSum_CsIPIDr 			&& make -is clean && cd ..)
	(cd ECalPosSum_CsIPIDr 			&& make -is clean && cd ..)
	(cd ECalParticleSumRunSum_CsIPIDr	&& make -is clean && cd ..)
	(cd EECalSum 				&& make -is clean && cd ..)
	(cd DeltaU 				&& make -is clean && cd ..)
	(cd DeltaUSum 				&& make -is clean && cd ..)
	(cd DeltaURingSum 				&& make -is clean && cd ..)
	(cd DeltaUSameRingSum 				&& make -is clean && cd ..)

	rm -f *~
