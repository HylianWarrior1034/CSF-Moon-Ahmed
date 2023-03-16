#
# Makefile for CSF Assignment 3
#

CXX = g++
CXX_SRCS = cachemain.cpp
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++11

LDFLAGS = -no-pie

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all : csim

csim : cachemain.o cachefuncs.o 
	$(CXX) -o $@ $(LDFLAGS) cachemain.o cachefuncs.o

csim.o: cachemain.cpp cachefuncs.h cachestruct.h
	$(CXX) -c $(CXXFLAGS) cachemain.cpp

cachefuncs.o: cachefuncs.cpp cachefuncs.h cachestruct.h
	$(CXX) -c $(CXXFLAGS) cachefuncs.cpp

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp

clean :
	rm -f *.o csim \
		depend.mak solution.zip

depend.mak :
	touch $@

depend :
	g++ -M $(CXX_SRCS) > depend.mak

include depend.mak
