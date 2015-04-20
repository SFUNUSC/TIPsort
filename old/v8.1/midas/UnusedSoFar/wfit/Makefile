all:

	echo making wfit_test_sig2noise                                      
	(cd test_sig2noise r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_test_sig2noise

	echo making wfit_test_T10T90                                     
	(cd test_T10T90 r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_test_T10T90

	echo making wfit_test_T50                                   
	(cd test_T50 r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_test_T50

	echo making wfit_test_T10                                   
	(cd test_T10 r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_test_T10

	echo making wfit_test_T90                                   
	(cd test_T90 r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: wfit_test_T90



clean:
	(cd test_sig2noise && make -is clean && cd ..)
	(cd test_T10T90 && make -is clean && cd ..)
	(cd test_T50 && make -is clean && cd ..)
	(cd test_T10 && make -is clean && cd ..)
	(cd test_T90 && make -is clean && cd ..)


	rm *~