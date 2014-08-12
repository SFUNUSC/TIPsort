all:
	echo Making ERaw.
	(cd ERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ERAW.

	echo Making ECal.
	(cd ECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ECAL.

clean:
	(cd ERaw && make -is clean && cd ..)
	(cd ECal && make -is clean && cd ..)
	rm -f *~