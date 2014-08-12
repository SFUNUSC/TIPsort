all:
	echo making S3_SecTRaw
	(cd SecTRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_SECTRAW

	echo making S3_SecERaw
	(cd SecERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_SECERAW

	echo making S3_RingTRaw
	(cd RingTRaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_RINGTRAW

	echo making S3_RingERaw
	(cd RingERaw r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_RINGERAW

	echo making S3_SecTCal
	(cd SecTCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_SECTCAL

	echo making S3_RingTCal
	(cd RingTCal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_RINGTCAL

	echo making S3_SecTCalLim
	(cd SecTCalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_SECTCALLIM

	echo making S3_RingTCalLim
	(cd RingTCalLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_RINGTCALLIM

	echo making S3_SecECal
	(cd SecECal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_SECECAL

	echo making S3_RingECal
	(cd RingECal r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_RINGECAL

	echo making S3_SecECalTLim
	(cd SecECalTLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_SECECALTLIM

	echo making S3_RingECalTLim
	(cd RingECalTLim r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: S3_RINGECALTLIM

clean:
	(cd SecTRaw && make -is clean && cd ..)
	(cd SecERaw && make -is clean && cd ..)
	(cd RingTRaw && make -is clean && cd ..)
	(cd RingERaw && make -is clean && cd ..)
	(cd SecTCal && make -is clean && cd ..)
	(cd RingTCal && make -is clean && cd ..)
	(cd SecTCalLim && make -is clean && cd ..)
	(cd RingTCalLim && make -is clean && cd ..)
	(cd SecECal && make -is clean && cd ..)
	(cd RingECal && make -is clean && cd ..)
	(cd SecECalTLim && make -is clean && cd ..)
	(cd RingECalTLim && make -is clean && cd ..)
	rm *~