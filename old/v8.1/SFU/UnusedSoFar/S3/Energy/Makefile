all:
	echo Making RingERaw.
	(cd RingERaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: RINGERAW.

	echo Making RingECal.
	(cd RingECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: RINGECAL.

clean:
	(cd RingERaw   && make -is clean && cd ..)
	(cd RingECal   && make -is clean && cd ..)
	rm -f *~