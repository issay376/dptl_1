# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/08/2004

include ../../makefile.inc

all : dptest

# -- temporal --
dptest : dptest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dptest -I${DPTLINC} -D_DEBUG_ALLOC ${CFLAGS} dptest.cpp

test : dptest
	./dptest 

clean :
	${RM} -f *.o
	${RM} -f dptest

