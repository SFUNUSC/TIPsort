all:
	echo making separate_TrigBits
	(cd TrigBits r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: SEPARATE_TRIGBITS

clean:
	(cd TrigBits && make -is clean && cd ..)

	rm *~