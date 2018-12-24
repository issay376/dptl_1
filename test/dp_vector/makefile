# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/03/2018

include ../../makefile.inc

all : dp_vectortest

# -- temporal --
dp_vectortest.cpp : ${DPTLBIN}/gendptest dp_vector.tmpl
	${DPTLBIN}/gendptest -s dp_vector.tmpl dp_vectortest.cpp

dp_vectortest : dp_vectortest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_vector.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_vectortest -I${DPTLINC} ${CFLAGS} dp_vectortest.cpp

test : dp_vectortest
	./dp_vectortest 

clean :
	${RM} -f *.o
	${RM} -f dp_vectortest
	${RM} -f dp_vectortest.cpp

