# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/03/2018

include ../../makefile.inc

all : dp_listtest

# -- temporal --
dp_listtest.cpp : ${DPTLBIN}/gendptest dp_list.tmpl
	${DPTLBIN}/gendptest -s -p dp_list dp_list.tmpl dp_listtest.cpp

dp_listtest : dp_listtest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_list.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_listtest -I${DPTLINC} ${CFLAGS} dp_listtest.cpp

test : dp_listtest
	./dp_listtest 

clean :
	${RM} -f *.o
	${RM} -f dp_listtest
	${RM} -f dp_listtest.cpp

