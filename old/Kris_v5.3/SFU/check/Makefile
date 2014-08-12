all:
	echo making check_Triggers
	(cd Triggers r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CHECK_TRIGGERS

	echo making check_TrigBits
	(cd TrigBits r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CHECK_TRIGBITS

	echo making check_EventTimeDifference
	(cd EventTimeDifference r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CHECK_EVENTTIMEDIFFERENCE

	echo making Rate
	(cd Rate r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CHECK_Rate

	echo making check_TimeStamps
	(cd TimeStamps r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CHECK_TIMESTAMPS
clean:
	(cd EventTimeDifference && make -is clean && cd ..)
	(cd Rate && make -is clean && cd ..)
	(cd TimeStamps && make -is clean && cd ..)
	(cd Triggers && make -is clean && cd ..)
	(cd TrigBits && make -is clean && cd ..)

	rm *~