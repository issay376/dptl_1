# Test Code Makefile for Deep Pointer
# 	K.Saitoh, 12/04/2018

include ../../makefile.inc

all : dp_unordered_maptest dp_unordered_multimaptest

# -- temporal --
dp_unordered_maptest.cpp : ${DPTLBIN}/gendptest dp_unordered_map.tmpl
	${DPTLBIN}/gendptest -s -p dp_unordered_map dp_unordered_map.tmpl dp_unordered_maptest.cpp

dp_unordered_maptest : dp_unordered_maptest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_unordered_map.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_unordered_maptest -I${DPTLINC} ${CFLAGS} dp_unordered_maptest.cpp

dp_unordered_multimaptest.cpp : ${DPTLBIN}/gendptest dp_unordered_multimap.tmpl
	${DPTLBIN}/gendptest -s -p dp_unordered_multimap dp_unordered_multimap.tmpl dp_unordered_multimaptest.cpp

dp_unordered_multimaptest : dp_unordered_multimaptest.cpp\
	${DPTLSRC}/include/deep_ptr.hpp\
	${DPTLSRC}/include/dp_unordered_map.hpp\
	${DPTLSRC}/include/utest.hpp\
	${DPTLSRC}/include/udata.hpp
	${CCPP} -o dp_unordered_multimaptest -I${DPTLINC} ${CFLAGS} dp_unordered_multimaptest.cpp

test : dp_unordered_maptest dp_unordered_multimaptest
	./dp_unordered_maptest
	./dp_unordered_multimaptest 

clean :
	${RM} -f *.o
	${RM} -f dp_unordered_maptest
	${RM} -f dp_unordered_maptest.cpp
	${RM} -f dp_unordered_multimaptest
	${RM} -f dp_unordered_multimaptest.cpp

