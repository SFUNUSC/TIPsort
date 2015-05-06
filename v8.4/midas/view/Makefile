all:
	echo Making view_fragment.
	(cd fragment r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_FRAGMENT.

	echo Making view_channel.
	(cd channel r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_CHANNEL.


clean:
	(cd fragment       && make -is clean && cd ..)
	(cd channel        && make -is clean && cd ..)

	rm -f *~