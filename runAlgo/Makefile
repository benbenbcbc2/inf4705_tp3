PROGRAM=runalgo
SOURCES=RunAlgo.cpp TableAndCompanies.cpp TrivialAlgo.cpp AlgoCotton1.cpp AlgoCotton2.cpp GraphAlgorithm.cpp
DEPS=Graph.h TableAndCompanies.h TrivialAlgo.h AlgoCotton1.h AlgoCotton2.h GraphAlgorithm.h filter_iterator.h
LIBS=-lTableCommon
LIBDIR=-L../common
HEADERS=-I../common

GCC=g++
CPPFLAGS=-std=c++11 -g -O0 -Wall -Werror

${PROGRAM} : ${SOURCES} ${DEPS}
	${GCC} ${CPPFLAGS} ${HEADERS} -o ${PROGRAM} ${SOURCES}  ${LIBDIR} ${LIBS}

clean:
	rm -rf *.o *.txt *.exe ${PROGRAM}
