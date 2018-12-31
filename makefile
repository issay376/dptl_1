# dptl Version 0.1

all :
	make -C util install
	make -C test

install :
	echo 'DPTLDIR = '`pwd` > makefile.inc
	cat makefile_inc >> makefile.inc
	if [ -d 'include' ];			\
	then 					\
		if [ -d 'include/dptl' ];	\
		then				\
			true;			\
		else				\
			ln -s `pwd`/src/include include/dptl;\
		fi				\
	else					\
		mkdir include;			\
		ln -s `pwd`/src/include include/dptl;\
	fi
	if [ -d 'bin' ]; then true; else mkdir bin; fi
	make -C util install

uninstall :
	make -C util uninstall

clean :
	make -C util clean
	make -C test clean

utest :
	make -C test test

