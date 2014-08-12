all:
	echo making TigressBGO_TTCal.
	(cd TTCal r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressBGO_TTCal.

	echo making TigressBGO_TTHP.
	(cd TTHP r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressBGO_TTHP.

	echo making TigressBGO_TTHP_Sum.
	(cd TTHP_Sum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressBGO_TTHP_Sum.

	echo making TigressBGO_TTHP_Map.
	(cd TTHP_Map r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressBGO_TTHP_Map.


clean:
	(cd TTCal && make -is clean && cd ..)
	(cd TTHP && make -is clean && cd ..)
	(cd TTHP_Sum && make -is clean && cd ..)
	(cd TTHP_Map && make -is clean && cd ..)

	rm -f *~
