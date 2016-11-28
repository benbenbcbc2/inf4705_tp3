PROGRAM=evalSol
SOURCES=EvalSol.cpp
LIBS=-lTableCommon
LIBDIR=-L../common
HEADERS=-I../common

GCC=g++
CPPFLAGS=-std=c++11 -g -O0 -Wall -Werror

${PROGRAM} : ${SOURCES}
	${GCC} ${CPPFLAGS} ${HEADERS} -o ${PROGRAM} ${SOURCES}  ${LIBDIR} ${LIBS}

clean:
	rm -rf *.o *.txt *.exe ${PROGRAM}
