# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/04/2018

include ../../makefile.inc

all : dp_settest dp_multisettest

# -- temporal --
dp_settest.cpp : ${DPTLBIN}/gendptest dp_set.tmpl
	${DPTLBIN}/gendptest -s -p dp_set dp_set.tmpl dp_settest.cpp

dp_settest : dp_settest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_set.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_settest -I${DPTLINC} ${CFLAGS} dp_settest.cpp

dp_multisettest.cpp : ${DPTLBIN}/gendptest dp_multiset.tmpl
	${DPTLBIN}/gendptest -s -p dp_multiset dp_multiset.tmpl dp_multisettest.cpp

dp_multisettest : dp_multisettest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_set.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_multisettest -I${DPTLINC} ${CFLAGS} dp_multisettest.cpp

test : dp_settest dp_multisettest
	./dp_settest
	./dp_multisettest 

clean :
	${RM} -f *.o
	${RM} -f dp_settest
	${RM} -f dp_settest.cpp
	${RM} -f dp_multisettest
	${RM} -f dp_multisettest.cpp

