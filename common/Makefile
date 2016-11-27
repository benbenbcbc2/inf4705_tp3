LIBNAME=TableCommon
SOURCES=TableAlgorithm.cpp

LIBFILE=lib${LIBNAME}.a

GCC=g++
CPPFLAGS=-std=c++11 -g -O0 -Wall -Werror

AR=ar
ARFLAGS=rcvs

${LIBFILE} : ${SOURCES}
	${GCC} ${CPPFLAGS} -c ${SOURCES}
	${AR} ${ARFLAGS} ${LIBFILE} $(SOURCES:.cpp=.o)

clean:
	rm -rf *.o *.a
