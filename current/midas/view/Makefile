all:
	echo Making view_fragment.
	(cd fragment r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_FRAGMENT.

	echo Making view_channel.
	(cd channel r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_CHANNEL.

	echo Making view_eventFragments.
	(cd eventFragments r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_EVENTFRAGMENTS.

clean:
	(cd fragment             && make -is clean && cd ..)
	(cd channel              && make -is clean && cd ..)
	(cd eventFragments       && make -is clean && cd ..)

	rm -f *~