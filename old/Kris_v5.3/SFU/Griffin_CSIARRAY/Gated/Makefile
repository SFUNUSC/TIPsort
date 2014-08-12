all:
	echo Making CsI_PID_TR.
	(cd CsI_PID_TR r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_PID_TR.

clean:
	(cd CsI_PID_TR && make -is clean && cd ..)
	rm -f *~