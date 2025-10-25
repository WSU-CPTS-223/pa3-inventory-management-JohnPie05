CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
INCLUDES = -Iinclude
APP = mainexe

SOURCES = main.cpp DataStore.cpp CSVReader.cpp

$(APP): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(APP)

clean:
	rm -f $(APP)
