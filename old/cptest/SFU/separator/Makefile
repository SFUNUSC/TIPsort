all:
	echo Making CsI_PID_RE.
	(cd CsI_PID_RE r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_PID_RE.

	echo Making CsI_PID_RT.
	(cd CsI_PID_RT r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_PID_RT.

	echo Making CsI_FitType.
	(cd CsI_FitType r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_FitType.

	echo Making Griffin_ECalTFitRaw.
	(cd Griffin_ECalTFitRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalTFitRaw.

	echo Making TrigBits.
	(cd TrigBits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TrigBits.

clean:
	(cd CsI_PID_RE          && make -is clean && cd ..)
	(cd CsI_PID_RT          && make -is clean && cd ..)
	(cd CsI_FitType         && make -is clean && cd ..)
	(cd Griffin_ECalTFitRaw && make -is clean && cd ..)
	(cd TrigBits            && make -is clean && cd ..)
	rm -f *~