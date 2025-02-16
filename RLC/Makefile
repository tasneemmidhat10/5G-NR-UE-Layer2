CXX = g++.exe
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -Werror
AR = ar.exe
ARFLAGS = rcs

SRC = transmitting.cpp receiving.cpp
OBJ = $(SRC:.cpp=.o)
LIB = rlc.lib

# Targets
all: main.exe profiler.exe

# Create static library
$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)
	del /F /Q $(OBJ)  # Clean object files after creating the library

# Compile main executable
main.exe: main.o $(LIB)
	$(CXX) $(CXXFLAGS) -o main.exe main.o $(LIB)
	del /F /Q main.o

# Compile profiler executable
profiler.exe: profiler.o $(LIB)
	$(CXX) $(CXXFLAGS) -o profiler.exe profiler.o $(LIB)
	del /F /Q profiler.o

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	del /F /Q *.o *.lib *.exe
