# Makefile definitions for Deep Pointer Template Library
#
#	Version 0.1 
#
# 	K.Saitoh, 2018/11/29
#
# --- Environment Depends -->
DPTLDIR = /Users/issay/work/dptl
INCDIR = /usr/local/include
LIBDIR = /usr/local/lib
# <-------------------------

DPTLSRC = ${DPTLDIR}/src
DPTLINC = ${DPTLDIR}/include
DPTLLIB = ${DPTLDIR}/lib
DPTLBIN = ${DPTLDIR}/bin

# use GCC 4.8 with -std=c++11 option
# CCPP = g++
CCPP = clang++

CFLAGS = -ggdb -D_DEBUG -Wno-parentheses -std=c++11 
# --- for udata.h printv() (native array print), c++14 is mandatory
# CFLAGS = -ggdb -D_DEBUG -Wno-parentheses -std=c++14 -D__cpp_lib_transformation_trait_aliases

CP = cp
RM = rm -f
AR = ar -cr

.cpp.o :
	${CCPP} ${CFLAGS} -I${DPTLINC} -c $<

