# Simple Makefile for mergesort

CXX = g++
CXXFLAGS = -O2 -std=c++11

all: mergesort

mergesort: mergesort.cpp
	$(CXX) $(CXXFLAGS) -o mergesort mergesort.cpp

clean:
	rm -f mergesort

