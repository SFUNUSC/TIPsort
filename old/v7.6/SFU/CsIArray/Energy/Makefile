all:
	echo Making ERaw.
	(cd ERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ERAW.

	echo Making ECal.
	(cd ECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECAL.

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


clean:
	(cd ERaw && make -is clean && cd ..)
	(cd ECal && make -is clean && cd ..)
	(cd ECal_CsIPIDr && make -is clean && cd ..)
	(cd ECalSum_CsIPIDr && make -is clean && cd ..)
	(cd ECalPosSum_CsIPIDr && make -is clean && cd ..)
	(cd ECalParticleSumRunSum_CsIPIDr && make -is clean && cd ..)


	rm -f *~