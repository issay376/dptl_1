# Test Code Makefile Template for Open Web Middleware
# 	K.Saitoh, 2018/12/30

include ../../makefile.inc

all : gendptest

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
