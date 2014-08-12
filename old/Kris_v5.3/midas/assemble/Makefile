all:
	echo making assemble_Tigress_CleanHP
	(cd TigressCleanHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_TIGRESSCLEANEHP

	echo making assemble_TigressCoreT
	(cd TigressCoreT r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_TIGRESSCORET

	echo making assemble_TigressSegT
	(cd TigressSegT r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_TIGRESSSEGT

	echo making assemble_PINArrayT
	(cd PINArrayT r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_PINArrayT

	echo making assemble_Clover_EHP
	(cd CloverEHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_CLOVEREHP

	echo making assemble_Clover_THP
	(cd CloverTHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_CLOVERTHP

	echo making assemble_Clover_CleanHP
	(cd CloverCleanHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_CLOVERCLEANEHP

	echo making assemble_TimeStampDifference
	(cd TimeStampDifference r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_TIMESTAMPDIFFERENCE

	echo making assemble_TrigBits
	(cd TrigBits r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_TRIGBITS

	echo making assemble_S3EHP
	(cd S3EHP r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: ASSEMBLE_S3EHP

clean:
	(cd TigressCleanHP && make -is clean && cd ..)
	(cd TigressCoreT && make -is clean && cd ..)
	(cd TigressSegT && make -is clean && cd ..)
	(cd PINArrayT && make -is clean && cd ..)
	(cd CloverEHP && make -is clean && cd ..)
	(cd CloverTHP && make -is clean && cd ..)
	(cd CloverCleanHP && make -is clean && cd ..)
	(cd TimeStampDifference && make -is clean && cd ..)
	(cd TrigBits && make -is clean && cd ..)
	(cd S3EHP && make -is clean && cd ..)
	rm *~