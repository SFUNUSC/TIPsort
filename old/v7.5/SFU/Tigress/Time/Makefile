all:
	echo making Tigress_TvsRFPhase.
	(cd TvsRFPhase r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TVSRFPHASE.

	echo making Tigress_TCal.
	(cd TCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TCal.

	echo making Tigress_TCalSum.
	(cd TCalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TCalSum.

clean:
	(cd TvsRFPhase && make -is clean && cd ..)
	(cd TCal && make -is clean && cd ..)
	(cd TCalSum && make -is clean && cd ..)
	rm -f *~