# Compiler flags
CXXFLAGS := -Wall -Wextra -std=c++14

# Include directories
INCLUDES := -I /usr/local/include 

# Linker flags
LDFLAGS := -L /usr/local/lib -lglfw3 -lvulkan -lobjc -framework CoreFoundation -framework CoreGraphics -framework Cocoa -framework IOKit -framework CoreVideo

# Source files
SRCS := main.cpp game_of_life.cpp debug_utils.cpp

# Header File 
HEADERS := game_of_life.hpp

# Object files
OBJS := $(addprefix objs/, $(SRCS:.cpp=.o))

# Executable name
TARGET := game_of_life

#NDEBUG set to 0 by default 
NDEBUG := 1

all: $(TARGET)
ifeq ($(NDEBUG), 0)
	@echo "Compiling in debug mode; Usage: ./game_of_life"
else
	@echo "Usage: ./game_of_life"
endif

$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DNDEBUG=$(NDEBUG) -o $@ $(OBJS) $(LDFLAGS)

objs/%.o: %.cpp $(HEADERS) | objs
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DNDEBUG=$(NDEBUG) -c $< -o $@

objs:
	@mkdir objs

debug:
	$(MAKE) all NDEBUG=0

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(OBJS) $(TARGET)
	$(RM) -rf objs

re: fclean all

.PHONY: all clean fclean re 