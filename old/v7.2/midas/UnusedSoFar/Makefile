all:

	echo making decoder
	(cd decoder && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: DECODER

	echo making state-machine
	(cd state-machine && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: STATE-MACHINE

	echo making map
	(cd map && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: MAP

	echo making assemble
	(cd assemble && make -is clean && make && make -is clean && cd ..)
	echo DONE: ASSEMBLE

	echo making project
	(cd project && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PROJECT

	echo making rawSpectra
	(cd rawSpectra && make -is clean && make && make -is clean && cd ..)
	echo DONE: RAWSPECTRA

	echo making RF
	(cd RF && make -is clean && make  && make -is clean && cd ..)
	echo DONE: RF

	echo making trigger
	(cd trigger && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TRIGGER

	echo making view
	(cd view && make -is clean && make  && make -is clean && cd ..)
	echo DONE: VIEW

	echo making wfit
	(cd wfit && make -is clean && make  && make -is clean && cd ..)
	echo DONE: WFIT

	echo making midas2sfu
	(cd midas2sfu && make -is clean && make  && make -is clean && cd ..)
	echo DONE: midas2sfu

clean:
	(cd assemble      && make -is clean && cd ..)
	(cd decoder       && make -is clean && cd ..)
	(cd state-machine && make -is clean && cd ..)
	(cd map           && make -is clean && cd ..)
	(cd project       && make -is clean && cd ..)
	(cd rawSpectra    && make -is clean && cd ..)
	(cd RF            && make -is clean && cd ..)
	(cd trigger       && make -is clean && cd ..)
	(cd view          && make -is clean && cd ..)
	(cd wfit          && make -is clean && cd ..)
	(cd midas2sfu && make -is clean && cd ..)
	rm *~
