# project name
PROJECT=        gir1

# Compiler
CXX=            g++

# C++ standard
CXX_STANDARD=   c++14

# Include directories
INC_DIR=        /usr/include \
				include \

# Library directories
LIB_DIR=        /usr/lib/

# Libraries:
# GL - OpenGL
# GLU - OpenGL Utility
# glfw - Managing window and input
# GLEW - Loads supported OpenGL functions for us
LIBS=           GL \
				GLU \
				glfw \
				GLEW \

# Compilation flags
CXXFLAGS=       -Wall \
				-std=$(CXX_STANDARD)

CXXFLAGS+=      $(addprefix -I, $(INC_DIR))

LDFLAGS=        $(addprefix -L, $(LIB_DIR)) \
				$(addprefix -l, $(LIBS))


HEADERS=        renderer \
				index_buffer \
				vertex_array \
				vertex_buffer \
				vertex_buffer_layout \
				version \
				defines \
				utils \


SOURCES=        renderer \
				index_buffer \
				vertex_array \
				vertex_buffer \
				version \
				utils


TEST_SOURCES=   test \


HEADERS :=      $(addprefix include/, $(addsuffix .h,   $(HEADERS)))
OBJECTS :=      $(addprefix build/,   $(addsuffix .o,   $(SOURCES)))
SOURCES :=      $(addprefix src/,     $(addsuffix .cpp, $(SOURCES)))

TEST_OBJECTS := $(addprefix build/,   $(addsuffix .o,   $(TEST_SOURCES)))
TEST_SOURCES := $(addprefix test/,    $(addsuffix .cpp, $(TEST_SOURCES)))

CURRENT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


# ================= RULES ================= #

# Unconditional rules
.PHONY: clean all test install uninstall version prebuild

# Silent rules
.SILENT: prebuild version install uninstall


# Build project and create symlink for easy access and run
all: prebuild version $(PROJECT)
	ln -sfn bin/$(PROJECT) run


#  Build main executable
$(PROJECT): main.cpp $(OBJECTS)
	g++ main.cpp $(OBJECTS) -o bin/$(PROJECT) $(CXXFLAGS) $(LDFLAGS)


# Build all object files
build/%.o: src/%.cpp $(HEADERS)
	g++ $< -c -o $@ $(CXXFLAGS)


# Compile test executable
bin/test: $(TEST_OBJECTS)
	g++ -o bin/test $(TEST_OBJECTS) $(CXXFLAGS) -lgtest -lgtest_main -pthread


# Compile test object files
build/%.o: tests/%.cpp $(HEADERS)
	g++ $< -c -o $@ $(CXXFLAGS)


# Run compiled tests
test: bin/test
	./bin/test


# Run script to generate version file
version:
	./version.sh


#
# Copy compiled executable into ~/.local/bin directory
#     (add this directory to PATH for convinience)
#
install: bin/$(PROJECT)
	mkdir -p ~/.local/bin
	cp --force --interactive --update --verbose $(CURRENT_DIR)bin/$(PROJECT) ~/.local/bin/$(PROJECT)


# Remove installed executable
uninstall:
	rm -f ~/.local/bin/$(PROJECT)


# Prebuild step
prebuild:
	mkdir -p bin
	mkdir -p build
	chmod a+x version.sh


# Remove all object files and executables, including symlink ./run
clean:
	find . -type f -name '*.o' -delete
	rm -fv bin/$(PROJECT)
	rm -fv bin/test
	rm -fv run
