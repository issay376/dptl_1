# Test Code Makefile Template for Open Web Middleware
# 	K.Saitoh, 12/08/2004

all : gendptest

PCTLDIR = /Users/issay/work/pctl
PCTLSRC = ${PCTLDIR}/src
PCTLINC = ${PCTLDIR}/include
PCTLBIN = ${PCTLDIR}/bin

DPTLDIR = /Users/issay/work/dptl
DPTLBIN = ${DPTLDIR}/bin

CCPP = clang++
CFLAGS = -ggdb -D_DEBUG -Wno-parentheses -std=c++11 

CP = cp
RM = rm -f
AR = ar -cr

gendptest : gendptest.cpp \
	${PCTLSRC}/include/pcontainer.h \
	${PCTLSRC}/include/plist.h
	${CCPP} -o gendptest -I${PCTLINC} ${CFLAGS} gendptest.cpp 

install : gendptest
	${CP} gendptest ${DPTLBIN}

uninstall : ${DPTLBIN}/gendptest
	${RM} ${DPTLBIN}/gendptest 

clean : 
	${RM} -f *.o
	${RM} -f gendptest
	${RM} -f test
