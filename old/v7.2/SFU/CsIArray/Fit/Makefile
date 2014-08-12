all:
	echo Making PID.
	(cd PID r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID.

	echo Making GoodnessOfFit.
	(cd GoodnessOfFit r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GoodnessOfFit.

clean:
	(cd PID           && make -is clean && cd ..)
	(cd GoodnessOfFit && make -is clean && cd ..)
	rm -f *~