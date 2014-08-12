export sort_TRIUMF_dir=/home/starosta/sort_TRIUMF
all:
	echo making utils
	(cd utils && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: UTILS

	echo making SFU-format
	(cd SFU-format && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: SFU-FORMAT

	echo making calibrator
	(cd calibrator && make -is very-clean && make  && make -is clean && cd ..)
	echo DONE: CALIBRATOR

	echo making midas
	(cd midas && make -is clean && make  && make -is clean && cd ..)
	echo DONE: MIDAS

#	echo making SFU
#	(cd SFU && make -is clean && make && make -is clean && cd ..)
#	echo DONE: SFU

clean:
	rm -r ../bin/midas/*
	rm -r ../bin/sfu/*
	(cd utils && make -is clean && cd ..)
	(cd SFU-format && make -is clean && cd ..)
	(cd calibrator && make -is clean && cd ..)
	(cd midas && make -is clean && cd ..)
	(cd SFU && make -is clean && cd ..)
	rm *~

very-clean:
	(cd utils && make -is very-clean && cd ..)
	(cd SFU-format && make -is very-clean && cd ..)
	(cd calibrator && make -is very-clean && cd ..)
	(cd midas && make -is clean && cd ..)
	(cd SFU && make -is clean && cd ..)
	rm *~