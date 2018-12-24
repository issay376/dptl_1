# Test Code Makefile Template for Open Web Middleware
# 	K.Saitoh, 12/08/2004

all : gendptest

DPTLDIR = /Users/issay/work/dptl
DPTLSRC = ${DPTLDIR}/src
DPTLINC = ${DPTLDIR}/include
DPTLBIN = ${DPTLDIR}/bin

CCPP = clang++
CFLAGS = -ggdb -D_DEBUG -Wno-parentheses -std=c++11 

CP = cp
RM = rm -f
AR = ar -cr

gendptest : gendptest.cpp \
	${DPTLSRC}/include/deep_ptr.hpp \
	${DPTLSRC}/include/dp_list.hpp
	${CCPP} -o gendptest -I${DPTLINC} ${CFLAGS} gendptest.cpp 

install : gendptest
	${CP} gendptest ${DPTLBIN}

uninstall : ${DPTLBIN}/gendptest
	${RM} ${DPTLBIN}/gendptest 

clean : 
	${RM} -f *.o
	${RM} -f gendptest
	${RM} -f test
