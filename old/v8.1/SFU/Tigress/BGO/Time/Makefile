all:
	echo Making BGO_TRaw.
	(cd TRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: BGO_ERAW.

	echo Making BGO_TRawSum.
	(cd TRawSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: BGO_ERAWSUM.

clean:
	(cd TRaw    && make -is clean && cd ..)
	(cd TRawSum && make -is clean && cd ..)
	rm -f *~