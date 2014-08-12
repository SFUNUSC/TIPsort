all:
	echo Making CsI_PIDer.
	(cd CsI_PID_ER r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_PIDer.

	echo Making CsI_PIDtr.
	(cd CsI_PID_TR r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_PIDtr.

	echo Making CsI_PIDfs.
	(cd CsI_PID_FS r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_PIDfs.

	echo Making CsI_FitType.
	(cd CsI_FitType r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CsI_FitType.

	echo Making Griffin_ECalTFitRaw.
	(cd Griffin_ECalTFitRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalTFitRaw.

	echo Making Griffin_ECalTFitRaw_Mult.
	(cd Griffin_ECalTFitRaw_Mult r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Griffin_ECalTFitRaw_Mult.

	echo Making Tigress_ECalTFitRaw.
	(cd Tigress_ECalTFitRaw r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_ECalTFitRaw.

	echo Making Tigress_TECal.
	(cd Tigress_TECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: Tigress_TECal.

	echo Making PINArray_TECal.
	(cd PINArray_TECal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINArray_TECal.

	echo Making TrigBits.
	(cd TrigBits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TrigBits.

clean:
	(cd CsI_PID_ER               && make -is clean && cd ..)
	(cd CsI_PID_TR               && make -is clean && cd ..)
	(cd CsI_PID_FS               && make -is clean && cd ..)
	(cd CsI_FitType              && make -is clean && cd ..)
	(cd Griffin_ECalTFitRaw      && make -is clean && cd ..)
	(cd Griffin_ECalTFitRaw_Mult && make -is clean && cd ..)
	(cd Tigress_ECalTFitRaw      && make -is clean && cd ..)
	(cd Tigress_TECal            && make -is clean && cd ..)
	(cd PINArray_TECal           && make -is clean && cd ..)
	(cd TrigBits                 && make -is clean && cd ..)
	rm -f *~