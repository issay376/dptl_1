# dptl Version 2.0

all :
	make -C util install
	make -C src
	make -C test

install :
	make -C src install

uninstall :
	make -C src uninstall

clean :
	make -C util clean
	make -C src clean
	make -C test clean

utest :
	make -C test test

