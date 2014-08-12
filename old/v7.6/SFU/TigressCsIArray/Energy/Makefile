all:
	echo Making ECalAB_CsIPIDtr.
	(cd ECalAB_CsIPIDtr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALAB_CSIPIDTR.

	echo Making ECalAB_CsIPIDr.
	(cd ECalAB_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALAB_CSIPIDR.

	echo Making ECalABSum_CsIPIDr.
	(cd ECalABSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUM_CSIPIDR.

	echo Making ECalABRingSum_CsIPIDr.
	(cd ECalABRingSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABRINGSUM_CSIPIDR.

	echo Making ECalABSuppRingSum_CsIPIDr.
	(cd ECalABSuppRingSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUPPRINGSUM_CSIPIDR.

	echo Making ECalABSum_CsIEGate_CsIPIDr.
	(cd ECalABSum_CsIEGate_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUM_CSIEGATE_CSIPIDR.

clean:
	(cd ECalAB_CsIPIDtr           && make -is clean && cd ..)
	(cd ECalAB_CsIPIDr            && make -is clean && cd ..)
	(cd ECalABSum_CsIPIDr         && make -is clean && cd ..)
	(cd ECalABRingSum_CsIPIDr     && make -is clean && cd ..)
	(cd ECalABSuppRingSum_CsIPIDr && make -is clean && cd ..)
	(cd ECalABSum_CsIEGate_CsIPIDr         && make -is clean && cd ..)

	rm -f *~