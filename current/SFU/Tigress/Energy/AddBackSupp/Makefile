all:
	echo Making Tigress_ECalABSupp.
	(cd ECalABSupp r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSupp.

	#echo Making Tigress_ECalABSuppTester.
	#(cd ECalABSuppTester r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABSuppTester.

	echo Making Tigress_ECalABSuppSum.
	(cd ECalABSuppSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppSum.

	echo Making Tigress_ECalABSuppRingSum.
	(cd ECalABSuppRingSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppRingSum.

	echo Making Tigress_ECalABSuppRingSumEGated.
	(cd ECalABSuppRingSumEGated r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppRingSumEGated.

	echo Making Tigress_ECalABSuppRingSumEGatedByRing.
	(cd ECalABSuppRingSumEGatedByRing r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalABSuppRingSumEGatedByRing.

	#echo Making Tigress_ECalABSuppLayerSum.
	#(cd ECalABSuppLayerSum r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABSuppLayerSum.

	#echo Making Tigress_ECalABSuppLayer.
	#(cd ECalABSuppLayer r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABSuppLayer.

	#echo Making Tigress_ABSuppPosFoldHP.
	#(cd SuppPosFoldHP r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ABSuppPosFoldHP.

	#echo Making Tigress_ECalABSupp_OnBGOE.
	#(cd SuppOnBGOELimits r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABSupp_OnBGOE.

	#echo Making Tigress_ECalABSupp_OnBGOT.
	#(cd SuppOnBGOTLimits r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABSupp_OnBGOT.

	echo Making Tigress_EECalABSuppSum.
	(cd EECalABSuppSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_EECalABSuppSum.

	#echo Making Tigress_ECalABSupp_OnBGOPresence.
	#(cd SuppOnBGOPresence r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Tigress_ECalABSupp_OnBGOPresence.

clean:
	(cd ECalABSupp               	    && make -is clean && cd ..)
	#(cd ECalABSuppTester         	    && make -is clean && cd ..)
	(cd ECalABSuppSum            	    && make -is clean && cd ..)
	(cd ECalABSuppRingSum               && make -is clean && cd ..)
	(cd ECalABSuppRingSumEGated         && make -is clean && cd ..)
	(cd ECalABSuppRingSumEGatedByRing   && make -is clean && cd ..)
	#(cd ECalABSuppLayerSum              && make -is clean && cd ..)
	#(cd SuppPosFoldHP                   && make -is clean && cd ..)
	#(cd SuppOnBGOELimits                && make -is clean && cd ..)
	#(cd SuppOnBGOTLimits                && make -is clean && cd ..)
	#(cd SuppOnBGOPresence               && make -is clean && cd ..)
	(cd EECalABSuppSum        	    && make -is clean && cd ..)

	rm -f *~
