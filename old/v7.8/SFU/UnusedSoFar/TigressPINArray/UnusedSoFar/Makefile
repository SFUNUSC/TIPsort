all:
	echo making Tigress_PINARRAY_TTCal
	(cd TTCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_PINARRAY_TTCal

	echo making Tigress_PINARRAY_TTCalLim
	(cd TTCalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_PINARRAY_TTCalLim

	echo making Tigress_PINARRAY_EECalLim
	(cd EECalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_PINARRAY_EECalLim

	echo making Tigress_PINARRAY_EECalLimCluster
	(cd EECalLimCluster r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_PINARRAY_EECalLimCluster

	echo making Tigress_PINARRAY_TigressRing_AllLim
	(cd TigressRing_AllLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Tigress_PINARRAY_TigressRing_AllLim


clean:
	(cd TTCal && make -is clean && cd ..)
	(cd TTCalLim && make -is clean && cd ..)
	(cd EECalLim && make -is clean && cd ..)
	(cd TigressRing_AllLim && make -is clean && cd ..)
	rm *~