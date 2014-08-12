all:
	echo making FFT_show                                               
	(cd show r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: FFT_show                                                

	echo making FFT_sum
	(cd sum r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: FFT_sum

clean:
	(cd show && make -is clean && cd ..)
	(cd sum && make -is clean && cd ..) 
	rm *~