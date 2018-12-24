# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/04/2018

include ../../makefile.inc

all : dp_maptest dp_multimaptest

# -- temporal --
dp_maptest.cpp : ${DPTLBIN}/gendptest dp_map.tmpl
	${DPTLBIN}/gendptest +s dp_map.tmpl dp_maptest.cpp

dp_maptest : dp_maptest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_map.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_maptest -I${DPTLINC} ${CFLAGS} dp_maptest.cpp

dp_multimaptest.cpp : ${DPTLBIN}/gendptest dp_multimap.tmpl
	${DPTLBIN}/gendptest +s dp_multimap.tmpl dp_multimaptest.cpp

dp_multimaptest : dp_multimaptest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_map.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_multimaptest -I${DPTLINC} ${CFLAGS} dp_multimaptest.cpp

test : dp_maptest dp_multimaptest
	./dp_maptest
	./dp_multimaptest 

clean :
	${RM} -f *.o
	${RM} -f dp_maptest
	${RM} -f dp_maptest.cpp
	${RM} -f dp_multimaptest
	${RM} -f dp_multimaptest.cpp

