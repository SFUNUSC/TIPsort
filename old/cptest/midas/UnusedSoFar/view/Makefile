all:
	echo making view_fragment
	(cd fragment r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: VIEW_FRAGMENT

	echo making view_channel
	(cd channel r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: VIEW_FRAGMENT

	echo making view_CFD
	(cd CFD r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: VIEW_CFD

clean:
	(cd fragment && make -is clean && cd ..)
	(cd channel && make -is clean && cd ..)
	(cd CFD && make -is clean && cd ..)
	rm *~