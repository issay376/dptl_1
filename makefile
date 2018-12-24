# dptl Version 2.0

all :
	make -C util install
	make -C test

install :
	make -C util install

uninstall :
	make -C util uninstall

clean :
	make -C util clean
	make -C test clean

utest :
	make -C test test

