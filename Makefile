# Makefile for NumXML
program = numxml
CXX = g++
CXXFLAGS = -Wall -g

$(program): main.o NumXML.o
	$(CXX) -o $(program) $(CXXFLAGS) main.o NumXML.o

main.o: main.cpp
	$(CXX) -c main.cpp $(CXXFLAGS) -o main.o

NumXML.o: NumXML.cpp NumXML.h
	$(CXX) -c NumXML.cpp -o $(CXXFLAGS) -o NumXML.o

.PHONY: clean
clean:
	rm $(program) NumXML.o main.o