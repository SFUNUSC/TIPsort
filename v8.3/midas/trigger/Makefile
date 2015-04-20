all:
	echo Making channelHP.
	(cd channelHP r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHANNELHP.

	echo Making frag_fold.
	(cd frag_fold r && make -is clean && make && make -is clean && cd ..)
	echo DONE: FRAGFOLD.

clean:
	(cd channelHP && make -is clean && cd ..)
	(cd frag_fold && make -is clean && cd ..)

	rm -f *~