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

tarball : 
	find . -type f | grep '^\.\/[^.].*\.' | grep -v '\.DS_Store' | grep -v '\.bk' | grep -v '\.sh' | grep -v '\.old' | grep -v '\.dSYM' | grep -v 'Info.plist' | grep -v '\.a' | grep -v 'makefile\.inc' > '$$tmpfile'
	find . -type f | grep 'makefile' | grep -v 'makefile\.inc' >> '$$tmpfile'
	cat '$$tmpfile' | xargs tar cvf - | gzip -c > '../bkup/dptl_release'`date '+%Y%m%d'`.tgz
	rm '$$tmpfile'

clean :
	make -C util clean
	make -C test clean

utest :
	make -C test test

