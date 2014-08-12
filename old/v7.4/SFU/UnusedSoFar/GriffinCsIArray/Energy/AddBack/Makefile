all:
	echo Making ECalAB_CsIPIDtr.
	(cd ECalAB_CsIPIDtr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALAB_CSIPIDTR.

	echo Making ECalABSum_CsIPIDtr.
	(cd ECalABSum_CsIPIDtr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALABSUM_CSIPIDTR.

clean:
	(cd ECalAB_CsIPIDtr    && make -is clean && cd ..)
	(cd ECalABSum_CsIPIDtr && make -is clean && cd ..)
	rm -f *~