all:
	echo making RF_showChisq                                               
	(cd showChisq r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: RF_showChisq                                                

	echo making RF_fitPeriod
	(cd fitPeriod r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RF_fitPeriod

	echo making RF_verify
	(cd verify r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RF_verify


clean:
	(cd showChisq && make -is clean && cd ..)
	(cd fitPeriod && make -is clean && cd ..) 
	(cd verify && make -is clean && cd ..)
	rm *~