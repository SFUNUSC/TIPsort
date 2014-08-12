CXX=g++

CXXFLAGS:=-g -Wall -I.
ROOTLIBS =$(shell root-config --nonew --glibs)
ROOTINC =-I$(shell root-config --nonew --incdir)
CXXFLAGS+=$(ROOTINC)
LIBS:=
OBJECTS:=main.cxx MainWindow.cxx RootManager.cxx
INCLUDES:=MainWindow.h
BIN = $(TIPsort)/bin

GateMaker: $(OBJECTS) $(INCLUDES)
	$(CXX) -o ${BIN}/GateMaker $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

clean: 
	rm -f *.o *Dict* *~ 