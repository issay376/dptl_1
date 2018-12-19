# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/03/2018

include ../../makefile.inc

all : dp_dequetest

# -- temporal --
dp_dequetest.cpp : ${DPTLBIN}/gendptest dp_deque.tmpl
	${DPTLBIN}/gendptest -s -p dp_deque dp_deque.tmpl dp_dequetest.cpp

dp_dequetest : dp_dequetest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_deque.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_dequetest -I${DPTLINC} ${CFLAGS} dp_dequetest.cpp

test : dp_dequetest
	./dp_dequetest 

clean :
	${RM} -f *.o
	${RM} -f dp_dequetest
	${RM} -f dp_dequetest.cpp

