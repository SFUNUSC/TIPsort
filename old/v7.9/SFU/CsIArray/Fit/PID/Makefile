all:
	echo Making PID_FS.
	(cd PID_FS r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_FS.

	echo Making PID_FS_Sum.
	(cd PID_FS_Sum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_FS_SUM.

	echo Making PID_ER.
	(cd PID_ER r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_ER.

	echo Making PID_ER_SUM.
	(cd PID_ER_Sum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_ER_SUM.

	echo Making PID_TR.
	(cd PID_TR r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_TR.

	echo Making PID_TR_SUM.
	(cd PID_TR_Sum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_TR_SUM.

	echo Making PID_ET.
	(cd PID_ET r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_ET.

	echo Making Gated_PIDtr.
	(cd Gated_PIDtr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Gated_PIDtr.

	echo Making PID_fitER.
	(cd PID_fitER r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PID_fitER.

clean:
	(cd PID_FS      && make -is clean && cd ..)
	(cd PID_FS_Sum  && make -is clean && cd ..)
	(cd PID_ER      && make -is clean && cd ..)
	(cd PID_ER_Sum  && make -is clean && cd ..)
	(cd PID_TR      && make -is clean && cd ..)
	(cd PID_TR_Sum  && make -is clean && cd ..)
	(cd PID_ET      && make -is clean && cd ..)
	(cd Gated_PIDtr && make -is clean && cd ..)
	(cd PID_fitER      && make -is clean && cd ..)
	rm -f *~