CXX=g++
CXXFLAGS:= -O -Wall -fPIC $(shell $(ROOT5SYS)/bin/root-config --cflags)
ROOT5LIBS = $(shell $(ROOT5SYS)/bin/root-config --glibs)
INC = -I$(ROOT5INC)
LIBS:= -lm -ldl
OBJECTS:=main.cxx MainWindow.cxx RootManager.cxx GMMsgBox.cxx GMTextEntry.cxx
INCLUDES:=MainWindow.h RootManager.h GMMsgBox.h GMTextEntry.h
BIN = $(TIPsort)/bin

GateMaker: $(OBJECTS) $(INCLUDES)
	$(CXX) -o $(BIN)/GateMaker $(OBJECTS) $(CXXFLAGS) $(INC) $(LIBS) $(ROOT5LIBS) 

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< $(CXXFLAGS) $(INC)

clean: 
	rm -f *.o *Dict* *~
