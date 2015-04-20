all:
	echo Making PINArray_TigECalLim.
	(cd TigECalLim r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY_TIGECALLIM.

	echo Making PINArray_TigECalLimSum.
	(cd TigECalLimSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY_TIGECALLIMSUM.

	echo Making PINArray_TigECalLimSumRing.
	(cd TigECalLimSumRing r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY_TIGECALLIMSUMRING.

clean:
	(cd TigECalLim        && make -is clean && cd ..)
	(cd TigECalLimSum     && make -is clean && cd ..)
	(cd TigECalLimSumRing && make -is clean && cd ..)
	rm -f *~