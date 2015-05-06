all:
	echo Making Energy.
	(cd Energy r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Energy.

	echo Making Time.
	(cd Time r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Time.

	echo Making Fit.
	(cd Fit r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: Fit.

clean:
	(cd Energy && make -is clean && cd ..)
	(cd Time   && make -is clean && cd ..)
	(cd Fit    && make -is clean && cd ..)
	rm -f *~