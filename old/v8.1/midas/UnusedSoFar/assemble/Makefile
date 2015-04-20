all:
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