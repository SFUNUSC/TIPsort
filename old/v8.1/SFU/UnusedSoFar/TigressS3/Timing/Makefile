all:
	echo making TigressS3_RingTCal
	(cd RingTCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressS3_RingTCal

	echo making TigressS3_SecTCal
	(cd SecTCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressS3_SecTCal

	echo making TigressS3_RingTCalSum
	(cd RingTCalSum r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressS3_RingTCalSum

	echo making TigressS3_SecTCalSum
	(cd SecTCalSum r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: TigressS3_SecTCalSum

clean:
	(cd RingTCal && make -is clean && cd ..)
	(cd SecTCal && make -is clean && cd ..)
	(cd RingTCalSum && make -is clean && cd ..)
	(cd SecTCalSum && make -is clean && cd ..)
	rm *~