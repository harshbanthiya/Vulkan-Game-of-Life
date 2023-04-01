# Compiler flags
CXXFLAGS := -Wall -Wextra -Werror 

# Include directories
INCLUDES := -I /usr/local/include 

# Linker flags
LDFLAGS := -L /usr/local/lib -lglfw3 -lvulkan -lobjc -framework CoreFoundation -framework CoreGraphics -framework Cocoa -framework IOKit -framework CoreVideo

# Source files
SRCS := main.cpp game_of_life.cpp 

# Header File 
HEADERS := game_of_life.hpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
TARGET := game_of_life

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)
