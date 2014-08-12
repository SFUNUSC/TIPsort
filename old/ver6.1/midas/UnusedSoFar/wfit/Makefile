all:
	echo making wfit_amplitude                                            
	(cd amplitude r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_amplitude

	echo making wfit_ATAN                                            
	(cd ATAN r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_ATAN

	echo making wfit_baseline                                            
	(cd baseline r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_baseline

                                                
                                                


clean:
	(cd amplitude && make -is clean && cd ..)

	rm *~