# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/03/2018

include ../../makefile.inc

all : dp_arraytest

# -- temporal --
dp_arraytest.cpp : ${DPTLBIN}/gendptest dp_array.tmpl
	${DPTLBIN}/gendptest -s dp_array.tmpl dp_arraytest.cpp

dp_arraytest : dp_arraytest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_array.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_arraytest -I${DPTLINC} ${CFLAGS} dp_arraytest.cpp

test : dp_arraytest
	./dp_arraytest 

clean :
	${RM} -f *.o
	${RM} -f dp_arraytest
	${RM} -f dp_arraytest.cpp

