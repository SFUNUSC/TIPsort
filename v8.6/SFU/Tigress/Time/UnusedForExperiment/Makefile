all:
	echo making Tigress_TRaw.
	(cd TRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TRaw.

	echo making Tigress_TRawSum.
	(cd TRawSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TRawSum.

	echo making Tigress_TCal.
	(cd TCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TCal.

	echo making Tigress_TCalSum.
	(cd TCalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TCalSum.

	echo making Tigress_TCalLimSum.
	(cd TCalLimSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TCalLimSum.

	echo making Tigress_RFUnwrapping.
	(cd RFUnwrapping r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_RFUnwrapping.

clean:
	(cd TRaw && make -is clean && cd ..)
	(cd TRawSum && make -is clean && cd ..)
	(cd TCal && make -is clean && cd ..)
	(cd TCalSum && make -is clean && cd ..)
	(cd TCalLimSum && make -is clean && cd ..)
	(cd RFUnwrapping && make -is clean && cd ..)
	rm -f *~