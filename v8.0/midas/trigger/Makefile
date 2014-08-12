all:
	echo Making channelHP.
	(cd channelHP r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHANNELHP.

clean:
	(cd channelHP && make -is clean && cd ..)
	rm -f *~