all:
	echo making TigressBGO_TTCalCFD.
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

	echo making TigressBGO_TDiff.
	(cd TDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressBGO_TDiff.

	echo making TigressBGO_TDiffSum.
	(cd TDiffSum r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressBGO_TDiffSum.

clean:
	(cd TTCalCFD && make -is clean && cd ..)
	(cd TTHP && make -is clean && cd ..)
	(cd TTHP_Sum && make -is clean && cd ..)
	(cd TTHP_Map && make -is clean && cd ..)
	(cd TDiff && make -is clean && cd ..)
	(cd TDiffSum && make -is clean && cd ..)


	rm -f *~
