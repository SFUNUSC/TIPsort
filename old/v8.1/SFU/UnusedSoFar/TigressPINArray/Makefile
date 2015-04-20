all:
	echo Making PINArray.
	(cd PINArray r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINARRAY.

	echo Making Tigress.
	(cd Tigress r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress.

	echo Making Time.
	(cd Time r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Time.

clean:
	(cd PINArray && make -is clean && cd ..)
	(cd Tigress  && make -is clean && cd ..)
	(cd Time     && make -is clean && cd ..)
	rm -f *~