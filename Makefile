CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -Wextra -g
INCLUDES  = -Iinclude
APP       = mainexe

SRC       = main.cpp DataStore.cpp CSVReader.cpp
OBJ       = $(SRC:.cpp=.o)
DEPS      = $(OBJ:.o=.d)

.PHONY: all clean clean-win run

all: $(APP)

$(APP): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Compile each .cpp to .o and also write a .d dependency file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Include generated dependency files if they exist
-include $(DEPS)

clean:
	rm -f $(OBJ) $(DEPS) $(APP)

# Windows PowerShell/CMD friendly cleaner
clean-win:
	-del /Q $(APP) 2>nul || exit 0
	-del /Q $(OBJ)  2>nul || exit 0
	-del /Q $(DEPS) 2>nul || exit 0

# Run the app with arguments: make run ARGS="path/to/file.csv"
run: $(APP)
	./$(APP) $(ARGS)
