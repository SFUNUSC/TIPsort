all:
	echo Making TTCalSum.
	(cd TTCalSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIGRESS_PINArray_TTCalSum.

clean:
	(cd TTCalSum && make -is clean && cd ..)
	rm -f *~