all:
	echo Making separator.
	(cd  separator && make -is clean && make && make -is clean && cd ..)
	echo DONE:SEPARATOR.

	echo Making Griffin.
	(cd  Griffin && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin.

	echo Making CsIArray.
	(cd  CsIArray && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsIArray.

	echo Making Check.
	(cd  check && make -is clean && make && make -is clean && cd ..)
	echo DONE: Check.

	echo Making Display.
	(cd  display && make -is clean && make && make -is clean && cd ..)
	echo DONE: Display.

clean:
	(cd separator && make -is clean && cd ..)
	(cd Griffin   && make -is clean && cd ..)
	(cd CsIArray  && make -is clean && cd ..)
	(cd check     && make -is clean && cd ..)
	(cd display   && make -is clean && cd ..)
	rm -f *~
