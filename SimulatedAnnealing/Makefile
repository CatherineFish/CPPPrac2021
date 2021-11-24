CXX = g++
CXXFLAGS += -std=c++17 -Wall -pedantic -Wextra -Wno-unused-variable
SOURCES =  generator.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE =  generator

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)