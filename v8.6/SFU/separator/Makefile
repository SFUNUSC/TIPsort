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

	echo Making Separator_TigressCsITTCalFirstHitDiff.
	(cd TigressCsITTCalFirstHitDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_TigressCsITTCalFirstHitDiff.

	echo Making Separator_CsITTCalDiff.
	(cd CsITTCalDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_CsITTCalDiff.

	#echo Making Separator_TigressCsITTCalDiff_firstEvent.
	#(cd TigressCsITTCalDiff_firstEvent r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: Separator_TigressCsITTCalDiff_firstEvent.

	echo Making Separator_CsIPIDER.
	(cd CsIPIDER r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_CsIPIDER.

	echo Making Separator_CsIPIDER_Sum.
	(cd CsIPIDER_Sum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_CsIPIDER_Sum.

	echo Making Separator_CsIDeltaU.
	(cd CsIDeltaU r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_CsIDeltaU.

	echo Making Separator_RF.
	(cd RF r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator_RF.

clean:
	(cd TrigBits            && make -is clean && cd ..)
	(cd TigressCsITTCalDiff && make -is clean && cd ..)
	(cd TigressCsITTCalFirstHitDiff && make -is clean && cd ..)
	(cd TigressTTCalDiff    && make -is clean && cd ..)
	(cd CsITTCalDiff        && make -is clean && cd ..)
	(cd CsIPIDER            && make -is clean && cd ..)
	(cd CsIPIDER_Sum        && make -is clean && cd ..)
	(cd CsIDeltaU        && make -is clean && cd ..)
	(cd RF                  && make -is clean && cd ..)
	#(cd TigressCsITTCalDiff_firstEvent	&& make -is clean && cd ..)

	rm -f *~
