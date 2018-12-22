# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/03/2018

include ../../makefile.inc

all : dp_forward_listtest

# -- temporal --
dp_forward_listtest.cpp : ${DPTLBIN}/gendptest dp_forward_list.tmpl
	${DPTLBIN}/gendptest -s -p dp_forward_list dp_forward_list.tmpl dp_forward_listtest.cpp

dp_forward_listtest : dp_forward_listtest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_forward_list.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_forward_listtest -I${DPTLINC} ${CFLAGS} dp_forward_listtest.cpp

test : dp_forward_listtest
	./dp_forward_listtest 

clean :
	${RM} -f *.o
	${RM} -f dp_forward_listtest
	${RM} -f dp_forward_listtest.cpp

