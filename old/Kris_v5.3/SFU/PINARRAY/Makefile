all:
	echo making PINARRAY_TRaw
	(cd TRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_TRaw

	echo making PINARRAY_TCal
	(cd TCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_TCal

	echo making PINARRAY_TCalLim
	(cd TCalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_TCalLim

	echo making PINARRAY_ERaw
	(cd ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_ERaw

	echo making PINARRAY_ECal
	(cd ECal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_ECal

	echo making PINARRAY_ECalLim
	(cd ECalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_ECalLim

	echo making PINARRAY_TECal
	(cd TECal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PINARRAY_TECal

clean:
	(cd TRaw && make -is clean && cd ..)
	(cd TCal && make -is clean && cd ..)
	(cd TCalLim && make -is clean && cd ..)
	(cd ERaw && make -is clean && cd ..)
	(cd ECal && make -is clean && cd ..)
	(cd ECalLim && make -is clean && cd ..)
	(cd TECal && make -is clean && cd ..)
	rm *~