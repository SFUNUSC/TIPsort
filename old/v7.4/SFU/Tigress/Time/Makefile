#Phil I am editing this so that I can work on the waveform analyzer with a
#version of the sort code that compiles properly. ASC

all:
	#echo Making NoUnwrapping.
	#(cd  NoUnwrapping r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: NOUNWRAPPING.

	#echo Making Unwrapping.
	#(cd Unwrapping r && make -is clean && make && make -is clean && cd ..)
	#echo DONE: UNWRAPPING.

	echo Making Safe.
	(cd Safe r && make -is clean && make && make -is clean && cd ..)
	echo DONE: SAFE.

clean:
	#(cd NoUnwrapping && make -is clean && cd ..)
	#(cd Unwrapping   && make -is clean && cd ..)
	(cd Safe   && make -is clean && cd ..)

	rm -f *~