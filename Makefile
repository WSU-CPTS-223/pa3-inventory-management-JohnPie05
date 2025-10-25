CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

APP = mainexe
SOURCES := $(wildcard src/*.cpp)

$(APP): $(SOURCES)
	$(CXX) $(CXXFLAGS) -Iinclude $(SOURCES) -o $(APP)

clean:
	rm -f $(APP)
