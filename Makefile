# project name
PROJECT = gir1

# Compiler
CXX = g++

# C++ standard
CXX_STANDARD = c++14

# Include directories
INC_DIR = \
	/usr/include \
	include \
	libs \

# Library directories
LIB_DIR = \
	/usr/lib/

# Prebuild library
LOCAL_LIBS = \
	stb \
	imgui \

# Libraries:
# GL - OpenGL
# GLU - OpenGL Utility
# glfw - Managing window and input
# GLEW - Loads supported OpenGL functions for us
LIBS = \
	GL \
	GLU \
	glfw \
	GLEW \

# Compilation flags
CXXFLAGS = \
	-Wall \
	-std=$(CXX_STANDARD)

CXXFLAGS += $(addprefix -I, $(INC_DIR))

LDFLAGS = \
	$(addprefix -L, $(LIB_DIR)) \
	$(addprefix -l, $(LIBS))


HEADERS = \
	renderer \
	index_buffer \
	vertex_array \
	vertex_buffer \
	vertex_buffer_layout \
	shader \
	texture \
	skybox \
	model \
	model_asset \
	model_instance \
	camera \
	input \
	version \
	defines \
	utils \


SOURCES = \
	renderer \
	index_buffer \
	vertex_array \
	vertex_buffer \
	vertex_buffer_layout \
	shader \
	texture \
	skybox \
	model \
	model_asset \
	model_instance \
	camera \
	input \
	version \
	utils \


TEST_SOURCES = \
	test \


HEADERS      := $(addprefix include/, $(addsuffix .h,   $(HEADERS)))
OBJECTS      := $(addprefix build/,   $(addsuffix .o,   $(SOURCES)))
SOURCES      := $(addprefix src/,     $(addsuffix .cpp, $(SOURCES)))

STATIC_LIBS  := $(foreach lib, $(LOCAL_LIBS), $(addprefix libs/$(lib)/bin/lib, $(addsuffix .a, $(lib))))

TEST_OBJECTS := $(addprefix build/,   $(addsuffix .o,   $(TEST_SOURCES)))
TEST_SOURCES := $(addprefix test/,    $(addsuffix .cpp, $(TEST_SOURCES)))

CURRENT_DIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


# ================= RULES ================= #

# Unconditional rules
.PHONY: $(PROJECT) debug test version prebuild postbuild clean clean_main clean_libs package

# Silent rules
.SILENT: prebuild postbuild version install uninstall


# Build project and create symlink for easy access and run
$(PROJECT): prebuild bin/$(PROJECT) postbuild

debug: prebuild bin/$(PROJECT)-debug

# ================== MAIN ================== #

#  Build main executable
bin/$(PROJECT): main.cpp $(OBJECTS) $(STATIC_LIBS)
	g++ main.cpp $(OBJECTS) $(STATIC_LIBS) -O2 -o bin/$(PROJECT) $(CXXFLAGS) $(LDFLAGS)

bin/$(PROJECT)-debug: main.cpp $(OBJECTS) $(STATIC_LIBS)
	g++ main.cpp $(OBJECTS) $(STATIC_LIBS) -g -o bin/$(PROJECT)-debug $(CXXFLAGS) $(LDFLAGS)

# Build all object files
build/%.o: src/%.cpp include/%.h
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


# Postbuild step
postbuild:
	ln -sfn bin/$(PROJECT) run


# Run script to generate version file
version: src/version.cpp .git/HEAD
	@chmod a+x version.sh

src/version.cpp:
	./version.sh


# Make package to ship
package: bin/$(PROJECT)
	rm -rf package
	mkdir -p package
	cp bin/$(PROJECT) package/$(PROJECT)
	cp -r resources package/
	zip -r gir1_$(shell ./version.sh).zip package


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
	rm -fv gir1_*.zip

clean_libs: $(LOCAL_LIBS)

$(LOCAL_LIBS):
	$(MAKE) -C $(addprefix libs/, $@) clean
