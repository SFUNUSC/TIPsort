all:
	echo making trig_idle
	(cd idle r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TRIG_IDLE

	echo making trig_diff
	(cd diff r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TRIG_DIFF

	echo making trig_frag_fold
	(cd frag_fold r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TRIG_FRAD_FOLD

	echo making trig_list
	(cd list r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TRIG_LIST

	echo making inspect
	(cd inspect r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TRIG_INSPECT

	echo making channelHP
	(cd channelHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: CHANNELHP




clean:
	(cd idle && make -is clean && cd ..)
	(cd diff && make -is clean && cd ..)
	(cd frag_fold && make -is clean && cd ..)
	(cd list && make -is clean && cd ..)
	(cd inspect && make -is clean && cd ..)
	(cd channelHP && make -is clean && cd ..)
	rm *~