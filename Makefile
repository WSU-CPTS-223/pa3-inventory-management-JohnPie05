	CXX = g++
CXXFLAGS = -std=c++14 -Wall -g

mainexe: src/main.cpp src/DataStore.cpp src/CSVReader.cpp src/Product.cpp
	$(CXX) $(CXXFLAGS) -Iinclude $^ -o $@

clean:
	rm -f mainexe