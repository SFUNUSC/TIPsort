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

	echo Making ECalABSuppSum_CsIPIDr.
	(cd ECalABSuppSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUPPSUM_CSIPIDR.

	echo Making ECalABRingSum_CsIPIDr.
	(cd ECalABRingSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABRINGSUM_CSIPIDR.

	echo Making ECalABSuppRingSum_CsIPIDr.
	(cd ECalABSuppRingSum_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUPPRINGSUM_CSIPIDR.

	echo Making ECalABSum_CsIEGate_CsIPIDr.
	(cd ECalABSum_CsIEGate_CsIPIDr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUM_CSIEGATE_CSIPIDR.

	echo Making ECalABSuppRingSum_deltaUcut.
	(cd ECalABSuppRingSum_deltaUcut r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUPPRINGSUM_DELTAUCUT.

	echo Making deltaU_ECalCut.
	(cd deltaU_ECalCut r && make -is clean && make && make -is clean && cd ..)
	echo DONE: DELTAU_ECALCUT.

	echo Making ECalABRing_CsIEGate.
	(cd  ECalABRing_CsIEGate r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABRING_CSIEGATE.

	echo Making ECalABRingSum_CsIEGate.
	(cd  ECalABRingSum_CsIEGate r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABRINGSUM_CSIEGATE.

	echo Making EE_Plunger.
	(cd  EE_Plunger r && make -is clean && make && make -is clean && cd ..)
	echo DONE: EE_PLUNGER.

	echo Making EE_PlungerSum.
	(cd  EE_PlungerSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: EE_PLUNGERSUM.

	echo Making EECal.
	(cd  EECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: EECAL.


clean:
	(cd ECalAB_CsIPIDtr                    && make -is clean && cd ..)
	(cd ECalAB_CsIPIDr                     && make -is clean && cd ..)
	(cd ECalABSum_CsIPIDr                  && make -is clean && cd ..)
	(cd ECalABSuppSum_CsIPIDr              && make -is clean && cd ..)
	(cd ECalABRingSum_CsIPIDr              && make -is clean && cd ..)
	(cd ECalABSuppRingSum_CsIPIDr          && make -is clean && cd ..)
	(cd ECalABSum_CsIEGate_CsIPIDr         && make -is clean && cd ..)
	(cd ECalABSuppRingSum_deltaUcut        && make -is clean && cd ..)
	(cd deltaU_ECalCut                     && make -is clean && cd ..)
	(cd ECalABRing_CsIEGate                && make -is clean && cd ..)
	(cd ECalABRingSum_CsIEGate             && make -is clean && cd ..)
	(cd EE_Plunger                         && make -is clean && cd ..)
	(cd EE_PlungerSum                      && make -is clean && cd ..)
	(cd EECal                              && make -is clean && cd ..)

	rm -f *~
