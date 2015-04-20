CXX=g++

CXXFLAGS:=-g -Wall -I.
ROOTLIBS =$(shell root-config --nonew --glibs)
ROOTINC =-I$(shell root-config --nonew --incdir)
CXXFLAGS+=$(ROOTINC)
LIBS:=
OBJECTS:=main.cxx MainWindow.cxx RootManager.cxx GMMsgBox.cxx GMTextEntry.cxx
INCLUDES:=MainWindow.h RootManager.h GMMsgBox.h GMTextEntry.h
BIN = $(TIPsort)/bin

GateMaker: $(OBJECTS) $(INCLUDES)
	$(CXX) -o ${BIN}/GateMaker $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS) 

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< 

clean: 
	rm -f *.o *Dict* *~ $(BIN)/GateMaker
