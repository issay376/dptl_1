# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/04/2018

include ../../makefile.inc

all : dp_unordered_settest dp_unordered_multisettest

# -- temporal --
dp_unordered_settest.cpp : ${DPTLBIN}/gendptest dp_unordered_set.tmpl
	${DPTLBIN}/gendptest dp_unordered_set.tmpl dp_unordered_settest.cpp

dp_unordered_settest : dp_unordered_settest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_unordered_set.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_unordered_settest -I${DPTLINC} ${CFLAGS} dp_unordered_settest.cpp

dp_unordered_multisettest.cpp : ${DPTLBIN}/gendptest dp_unordered_multiset.tmpl
	${DPTLBIN}/gendptest dp_unordered_multiset.tmpl dp_unordered_multisettest.cpp

dp_unordered_multisettest : dp_unordered_multisettest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_unordered_set.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_unordered_multisettest -I${DPTLINC} ${CFLAGS} dp_unordered_multisettest.cpp

test : dp_unordered_settest dp_unordered_multisettest
	./dp_unordered_settest
	./dp_unordered_multisettest 

clean :
	${RM} -f *.o
	${RM} -f dp_unordered_settest
	${RM} -f dp_unordered_settest.cpp
	${RM} -f dp_unordered_multisettest
	${RM} -f dp_unordered_multisettest.cpp

