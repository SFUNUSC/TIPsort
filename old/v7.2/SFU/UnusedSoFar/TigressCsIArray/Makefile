all:
	echo making Tigress_CSIARRAY_EECalCluster
	(cd EECalCluster r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_CSIARRAY_EECalCluster

	echo making Tigress_CSIARRAY_TigressSuppRing_AllLim
	(cd TigressSuppRing_AllLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressSuppRing_AllLim

	echo making Tigress_CSIARRAY_TigressSuppRing_AllLimCluster
	(cd TigressSuppRing_AllLimCluster r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressSuppRing_AllLimCluster

	echo making Tigress_CSIARRAY_TigressSuppRing_TTDiffCluster
	(cd TigressSuppRing_TTDiffCluster r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressSuppRing_TTDiffCluster

	echo making Tigress_CSIARRAY_TTCal
	(cd TTCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_CSIARRAY_TTCal

	echo making Tigress_CSIARRAY_TTCalLim
	(cd TTCalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_CSIARRAY_TTCalLim

	echo making Tigress_CSIARRAY_TTRawDiffCluster
	(cd TTRawDiffCluster r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_CSIARRAY_TTRawDiffCluster


clean:
	(cd EECalCluster && make -is clean && cd ..)
	(cd TTCal && make -is clean && cd ..)
	(cd TTCalLim && make -is clean && cd ..)
	rm *~