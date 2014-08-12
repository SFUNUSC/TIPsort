all:
	echo Making TrigBits.
	(cd TrigBits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TrigBits.

	echo Making Separator_TigressCsITTCalDiff.
	(cd TigressCsITTCalDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_TigressCsITTCalDiff.

	echo Making Separator_TigressTTCalDiff.
	(cd TigressTTCalDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_TigressTTCalDiff.

	echo Making Separator_CsITTCalDiff.
	(cd CsITTCalDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_CsITTCalDiff.

	echo Making Separator_TigressCsITTCalDiff_firstEvent.
	(cd TigressCsITTCalDiff_firstEvent r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_TigressCsITTCalDiff_firstEvent.

clean:
	(cd TrigBits            && make -is clean && cd ..)
	(cd TigressCsITTCalDiff && make -is clean && cd ..)
	(cd TigressTTCalDiff    && make -is clean && cd ..)
	(cd CsITTCalDiff        && make -is clean && cd ..)
	(cd TigressCsITTCalDiff_firstEvent	&& make -is clean && cd ..)

	rm -f *~