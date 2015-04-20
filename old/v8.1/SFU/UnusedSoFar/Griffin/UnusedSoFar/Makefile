all:

	echo making Griffin_ERaw
	(cd ERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_ERaw

	echo making Griffin_ECal
	(cd ECal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_ECal

	echo making Griffin_ESegRaw
	(cd ESegRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_ESegRaw

	echo making Griffin_GePosFold
	(cd GePosFold r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_GEPOSFOLD

	echo making Griffin_GePosHP
	(cd GePosHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_GEPOSHP

	echo making Griffin_TRaw
	(cd TRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_TRaw

	echo making Griffin_TCal
	(cd TCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_TCal

	echo making Griffin_TGeGe
	(cd TGeGe r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: GRIFFIN_TGeGe


clean:
	(cd ESegRaw && make -is clean && cd ..)
	(cd ERaw && make -is clean && cd ..)
	(cd ECal && make -is clean && cd ..)
	(cd GePosFold && make -is clean && cd ..)
	(cd GePosHP && make -is clean && cd ..)
	(cd TRaw && make -is clean && cd ..)
	(cd TCal && make -is clean && cd ..)
	(cd TGeGe && make -is clean && cd ..)

	rm *~