all:
	echo Making TrigBits.
	(cd TrigBits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TrigBits.

clean:
	(cd TrigBits                 && make -is clean && cd ..)
	rm -f *~