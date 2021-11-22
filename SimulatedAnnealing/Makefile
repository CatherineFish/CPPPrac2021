CXX = g++
CXXFLAGS += -std=c++17 -Wall -pedantic -Wextra -Wno-unused-variable
SOURCES = main.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
LIB = mainAlgorithm.h solution.h random.h mutation.h 
EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIB) $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)