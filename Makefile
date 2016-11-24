PROGRAM=runalgo
SOURCES=RunAlgo.cpp TableAlgorithm.h TableAlgorithm.cpp

GCC=g++
CPPFLAGS=-std=c++11 -g -O0 -Wall -Werror

${PROGRAM} : ${SOURCES}
	${GCC} ${CPPFLAGS} -o ${PROGRAM} ${SOURCES}
