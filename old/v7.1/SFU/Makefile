all:
	echo Making separator.
	(cd  separator && make -is clean && make && make -is clean && cd ..)
	echo DONE:SEPARATOR.

	echo Making Griffin.
	(cd  Griffin && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin.

	echo Making Tigress.
	(cd  Tigress && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESS.

	echo Making CsIArray.
	(cd  CsIArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsIArray.

	echo Making PINArray.
	(cd  PINArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINArray.

	echo Making Check.
	(cd  check && make -is clean && make && make -is clean && cd ..)
	echo DONE: Check.

	echo Making Display.
	(cd  display && make -is clean && make && make -is clean && cd ..)
	echo DONE: Display.

	echo Making TigressPINArray.
	(cd  TigressPINArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESSPINARRAY.

	echo Making GriffinCsIArray.
	(cd  GriffinCsIArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFINCSIARRAY.

	echo Making S3.
	(cd  S3 && make -is clean && make && make -is clean && cd ..)
	echo DONE: S3.

clean:
	(cd separator       && make -is clean && cd ..)
	(cd Griffin         && make -is clean && cd ..)
	(cd Tigress         && make -is clean && cd ..)
	(cd CsIArray        && make -is clean && cd ..)
	(cd PINArray        && make -is clean && cd ..)
	(cd check           && make -is clean && cd ..)
	(cd display         && make -is clean && cd ..)
	(cd TigressPINArray && make -is clean && cd ..)
	(cd GriffinCsIArray && make -is clean && cd ..)
	(cd S3              && make -is clean && cd ..)
	rm -f *~
