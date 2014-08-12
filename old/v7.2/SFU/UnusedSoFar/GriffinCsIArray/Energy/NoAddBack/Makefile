all:
	echo Making ECal_CsIPIDtr.
	(cd ECal_CsIPIDtr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECAL_CSIPIDTR.

	echo Making ECalSum_CsIPIDtr.
	(cd ECalSum_CsIPIDtr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECALSUM_CSIPIDTR.

clean:
	(cd ECal_CsIPIDtr    && make -is clean && cd ..)
	(cd ECalSum_CsIPIDtr && make -is clean && cd ..)
	rm -f *~