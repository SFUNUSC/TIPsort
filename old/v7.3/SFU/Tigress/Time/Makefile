all:
	echo Making NoUnwrapping.
	(cd  NoUnwrapping r && make -is clean && make && make -is clean && cd ..)
	echo DONE: NOUNWRAPPING.

	echo Making Unwrapping.
	(cd Unwrapping r && make -is clean && make && make -is clean && cd ..)
	echo DONE: UNWRAPPING.

clean:
	(cd NoUnwrapping && make -is clean && cd ..)
	(cd Unwrapping   && make -is clean && cd ..)
	rm -f *~