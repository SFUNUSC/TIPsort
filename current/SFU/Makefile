all:
	echo Making Tigress.
	(cd  Tigress && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESS.
	
	echo Making TigressCsIArray.
	(cd  TigressCsIArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESSCSIARRAY.
	
	echo Making TigressBGO.
	(cd  TigressBGO && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESSBGO.
	
	echo Making CsIArray.
	(cd  CsIArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsIArray.
	
	echo Making Check.
	(cd  check && make -is clean && make && make -is clean && cd ..)
	echo DONE: Check.
	
	echo Making Display.
	(cd  display && make -is clean && make && make -is clean && cd ..)
	echo DONE: Display.
	
	echo Making Separator.
	(cd separator && make -is clean && make && make -is clean && cd ..)
	echo DONE: Separator.
	
	echo Making AddSFU.
	(cd  AddSFU && make -is clean && make && make -is clean && cd ..)
	echo DONE: ADDSFU.

clean:
	(cd Tigress         && make -is clean && cd ..)
	(cd TigressCsIArray && make -is clean && cd ..)
	(cd TigressBGO	    && make -is clean && cd ..)
	(cd CsIArray        && make -is clean && cd ..)
	(cd check           && make -is clean && cd ..)
	(cd separator       && make -is clean && cd ..)
	(cd display         && make -is clean && cd ..)	
	(cd AddSFU          && make -is clean && cd ..)	
	rm -f *~
