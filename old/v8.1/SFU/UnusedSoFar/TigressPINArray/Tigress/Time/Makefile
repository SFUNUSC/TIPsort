all:
	echo Making Tigress_TRawSum_wrtPIN.
	(cd Tigress_TRawSum_wrtPIN r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressPINArray_Tigress_TRawSum_wrtPIN.

	echo Making Tigress_TCalSum_wrtPIN.
	(cd Tigress_TCalSum_wrtPIN r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TigressPINArray_Tigress_TCalSum_wrtPIN.

	echo Making PINArray_TRawSum_wrtTigress.
	(cd PINArray_TRawSum_wrtTigress r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINArray_TRawSum_wrtTigress.

	echo Making PINArray_TCalSum_wrtTigress.
	(cd PINArray_TCalSum_wrtTigress r && make -is clean && make && make -is clean && cd ..)
	echo DONE: PINArray_TCalSum_wrtTigress.

clean:
	(cd Tigress_TRawSum_wrtPIN      && make -is clean && cd ..)
	(cd Tigress_TCalSum_wrtPIN      && make -is clean && cd ..)
	(cd PINArray_TRawSum_wrtTigress && make -is clean && cd ..)
	(cd PINArray_TCalSum_wrtTigress && make -is clean && cd ..)
	rm -f *~