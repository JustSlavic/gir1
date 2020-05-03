# project name
PROJECT=        gir1

# Compiler
CXX=            g++

# C++ standard
CXX_STANDARD=   c++14

# Include directories
INC_DIR=        /usr/include \
				include \
				libs \

# Library directories
LIB_DIR=        /usr/lib/

# Prebuild library
LOCAL_LIBS=     stb \

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


HEADERS=        \
				renderer \
				index_buffer \
				vertex_array \
				vertex_buffer \
				vertex_buffer_layout \
				shader \
				texture \
				version \
				defines \
				utils \


SOURCES=        \
				renderer \
				index_buffer \
				vertex_array \
				vertex_buffer \
				vertex_buffer_layout \
				shader \
				texture \
				version \
				utils \


TEST_SOURCES=   test \


HEADERS      := $(addprefix include/, $(addsuffix .h,   $(HEADERS)))
OBJECTS      := $(addprefix build/,   $(addsuffix .o,   $(SOURCES)))
SOURCES      := $(addprefix src/,     $(addsuffix .cpp, $(SOURCES)))

STATIC_LIBS  := $(foreach lib, $(LOCAL_LIBS), $(addprefix libs/$(lib)/bin/lib, $(addsuffix .a, $(lib))))

TEST_OBJECTS := $(addprefix build/,   $(addsuffix .o,   $(TEST_SOURCES)))
TEST_SOURCES := $(addprefix test/,    $(addsuffix .cpp, $(TEST_SOURCES)))

CURRENT_DIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


# ================= RULES ================= #

# Unconditional rules
.PHONY: all test install uninstall version prebuild postbuild clean clean_main clean_libs

# Silent rules
.SILENT: prebuild postbuild version install uninstall


# Build project and create symlink for easy access and run
all: prebuild $(PROJECT) postbuild
	ln -sfn bin/$(PROJECT) run

# ================== MAIN ================== #

#  Build main executable
$(PROJECT): main.cpp $(OBJECTS) $(STATIC_LIBS)
	g++ main.cpp $(OBJECTS) $(STATIC_LIBS) -o bin/$(PROJECT) $(CXXFLAGS) $(LDFLAGS)

# Build all object files
build/%.o: src/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	g++ $< -c -o $@ $(CXXFLAGS)


# ============== LOCAL LIBRARY ============== #

${STATIC_LIBS}:
	$(MAKE) -C $(shell dirname $(dir $@))

# ================= TESTS ================= #

# Compile test executable
bin/test: $(TEST_OBJECTS)
	@mkdir -p bin
	g++ -o bin/test $(TEST_OBJECTS) $(CXXFLAGS) -lgtest -lgtest_main -pthread


# Compile test object files
build/%.o: tests/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	g++ $< -c -o $@ $(CXXFLAGS)


# Run compiled tests
test: bin/test
	@echo "Running tests..."
	@./bin/test


# ================= UTILITY ================= #

# Prebuild step
prebuild: version
	mkdir -p bin
	chmod a+x version.sh


# Postbuild step
postbuild: ;


# Run script to generate version file
version:
	./version.sh


#
# Copy compiled executable into ~/.local/bin directory
#     (add this directory to PATH for convinience)
#
install: bin/$(PROJECT)
	@mkdir -p ~/.local/bin
	cp --force --interactive --update --verbose $(CURRENT_DIR)bin/$(PROJECT) ~/.local/bin/$(PROJECT)


# Remove installed executable
uninstall:
	rm -f ~/.local/bin/$(PROJECT)


#
# Cleaning
#

.PHONY: $(LOCAL_LIBS)

clean: clean_main clean_libs

clean_main:
	find build -type f -name '*.o' -delete
	rm -fv bin/$(PROJECT)
	rm -fv bin/test
	rm -fv run

clean_libs: $(LOCAL_LIBS)

$(LOCAL_LIBS):
	$(MAKE) -C $(addprefix libs/, $@) clean
