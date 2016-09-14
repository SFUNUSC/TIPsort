all:
	echo Making project_channel.
	(cd channel r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PROJECT_CHANNEL.

	echo Making project_tig10.
	(cd tig10 r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: PROJECT_TIG10.

clean:
	(cd channel    && make -is clean && cd ..)
	(cd tig10      && make -is clean && cd ..)
	rm -f *~