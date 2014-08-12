all:
	echo Making Griffin_TCal.
	(cd TCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: GRIFFIN_TCal.

clean:
	(cd TCal    && make -is clean && cd ..)
	rm -f *~