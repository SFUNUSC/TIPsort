all:
	echo making CSIARRAY_TRaw
	(cd TRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CSIARRAY_TRaw

	echo making CSIARRAY_TFitRaw
	(cd TFitRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CSIARRAY_TFitRaw

	echo making CSIARRAY_ERaw
	(cd ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CSIARRAY_ERaw

	echo making CSIARRAY_FitChisq
	(cd FitChisq r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CSIARRAY_FitChisq

	echo making CSIARRAY_DumpFitPar
	(cd DumpFitPar r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CSIARRAY_DumpFitPar


clean:
	(cd TRaw && make -is clean && cd ..)
	(cd ERaw && make -is clean && cd ..)
	(cd TFitRaw && make -is clean && cd ..)
	(cd FitChisq && make -is clean && cd ..)
	(cd DumpFitPar && make -is clean && cd ..)
	rm *~