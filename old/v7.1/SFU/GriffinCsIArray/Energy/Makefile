all:
	echo Making Energy_NoAddBack Programs.
	(cd NoAddBack r && make -is clean && make && make -is clean && cd ..)
	echo DONE: NOADDBACK.

	echo Making Energy_AddBack Programs.
	(cd AddBack r && make -is clean && make && make -is clean && cd ..)
	echo DONE: ADDBACK.

clean:
	(cd NoAddBack && make -is clean && cd ..)
	(cd AddBack   && make -is clean && cd ..)
	rm -f *~