CXX = g++
CXXFLAGS += -std=c++17 -lgtest -pthread
SOURCES = main.cpp factory.cpp graphLib.cpp findFunction.cpp
OBJECTS = $(SOURCES:.cpp=.o) 
EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)
 

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) -o $(EXECUTABLE) $(OBJECTS) $(CXXFLAGS) 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

