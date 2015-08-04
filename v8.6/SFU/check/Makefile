all:
	echo Making check_Triggers.
	(cd Triggers r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TRIGGERS.

	echo Making check_TrigBits.
	(cd TrigBits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TRIGBITS.

	echo Making check_Fold.
	(cd Fold r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_FOLD.

	echo Making check_Fold2D.
	(cd Fold2D r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_FOLD2D.

	echo Making check_Fold2D_PIDER.
	(cd Fold2D_PIDER r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_FOLD2D_PIDER.

	echo Making check_Fold2D_PIDERSUM.
	(cd Fold2D_PIDERSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_FOLD2D_PIDERSUM.

	echo Making check_FoldSum.
	(cd FoldSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_FOLDSUM.

	echo Making check_EventTimeDifference.
	(cd EventTimeDifference r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_EVENTTIMEDIFFERENCE.

	echo Making Rate.
	(cd Rate r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_RATE.

	echo Making check_TimeStamps.
	(cd TimeStamps r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TIMESTAMPS.

	echo Making check_TimeStampDifference.
	(cd TimeStampDifference r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TIMESTAMPDIFFERENCE.

	echo Making check_TimeGatedTrigger.
	(cd TimeGatedTrigger r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TIMEGATEDTRIGGER.

clean:
	(cd EventTimeDifference && make -is clean && cd ..)
	(cd Rate                && make -is clean && cd ..)
	(cd TimeStamps          && make -is clean && cd ..)
	(cd Triggers            && make -is clean && cd ..)
	(cd TrigBits            && make -is clean && cd ..)
	(cd Fold                && make -is clean && cd ..)
	(cd Fold2D              && make -is clean && cd ..)
	(cd Fold2D_PIDER        && make -is clean && cd ..)
	(cd Fold2D_PIDERSum     && make -is clean && cd ..)
	(cd FoldSum             && make -is clean && cd ..)
	(cd TimeStampDifference && make -is clean && cd ..)
	(cd TimeGatedTrigger    && make -is clean && cd ..)

	rm -f *~
