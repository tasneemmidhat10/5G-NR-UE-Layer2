CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -Werror
AR = ar
ARFLAGS = rcs

SRC = transmitting.cpp receiving.cpp
OBJ = $(SRC:.cpp=.o)
LIB = librlc.a

# Targets
all: rlc_sim profiler

# Create static library
$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)
	rm -f $(OBJ)  # Clean object files after creating the library

# Compile main executable
rlc_sim: main.o $(LIB)
	$(CXX) $(CXXFLAGS) -o rlc_sim main.o $(LIB)
	rm -f main.o

# Compile profiler executable
profiler: profiler.o $(LIB)
	$(CXX) $(CXXFLAGS) -o profiler profiler.o $(LIB)
	rm -f profiler.o

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f *.o *.a rlc_sim profiler

