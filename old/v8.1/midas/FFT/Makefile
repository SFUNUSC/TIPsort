all:
	echo Making FFT_show.                                               
	(cd show r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: FFT_show.                                                

	echo Making FFT_sum.
	(cd sum r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: FFT_sum.

	echo Making FFT_CsI_Fit.
	(cd CsI_fit r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: FFT_CsI_Fit.

	echo Making FFT_Baseline_Fit.
	(cd baseline_fit r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: FFT_Baseline_Fit.

clean:
	(cd show         && make -is clean && cd ..)
	(cd sum          && make -is clean && cd ..) 
	(cd CsI_fit      && make -is clean && cd ..) 
	(cd baseline_fit && make -is clean && cd ..) 
	rm -f *~