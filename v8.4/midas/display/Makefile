all:
	echo Making display_rawEvent.
	(cd rawEvent r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RAWEVENT

clean:
	(cd rawEvent      && make -is clean && cd ..)
	rm -f *~