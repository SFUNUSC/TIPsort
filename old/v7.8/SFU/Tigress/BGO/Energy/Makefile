all:
	echo Making BGO_ERaw.
	(cd ERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: BGO_ERAW.

	echo Making BGO_ERawSum.
	(cd ERawSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: BGO_ERAWSUM.

clean:
	(cd ERaw    && make -is clean && cd ..)
	(cd ERawSum && make -is clean && cd ..)
	rm -f *~