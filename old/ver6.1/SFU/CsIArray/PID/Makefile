all:
	echo Making PID_FS.
	(cd PID_FS r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_FS.

	echo Making PID_ER.
	(cd PID_ER r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_ER.

	echo Making PID_TR.
	(cd PID_TR r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_TR.

	echo Making PID_ET.
	(cd PID_ET r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_ET.

clean:
	(cd PID_FS && make -is clean && cd ..)
	(cd PID_ER && make -is clean && cd ..)
	(cd PID_TR && make -is clean && cd ..)
	(cd PID_ET && make -is clean && cd ..)
	rm -f *~