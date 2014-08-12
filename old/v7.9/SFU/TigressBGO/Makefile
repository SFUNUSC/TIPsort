all:
	echo Making Time.
	(cd Time r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESSBGO_TIME.


clean:
	(cd Time && make -is clean && cd ..)
	rm -f *~