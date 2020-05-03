PROJECT=imgui
CXX=g++
CXX_STANDARD=c++14


HEADERS= \
         imconfig \
         imgui \
         imgui_internal \
         imstb_rectpack \
         imstb_textedit \
         imstb_truetype \
         imgui_impl_glfw \
         imgui_impl_opengl3 \


SOURCES= \
         imgui \
         imgui_impl_glfw \
         imgui_impl_opengl3 \
         imgui_draw \
         imgui_widgets \
#          imgui_demo \


HEADERS      := $(addsuffix .h,   $(HEADERS))
OBJECTS      := $(addprefix build/,   $(addsuffix .o,   $(SOURCES)))
SOURCES      := $(addprefix src/,     $(addsuffix .cpp, $(SOURCES)))


.PHONY: clean

all: $(OBJECTS)
	@mkdir -p bin
	ar rcvs bin/lib$(PROJECT).a $(OBJECTS)

# Build all object files
build/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	g++ $< -c -o $@ $(CXXFLAGS)


clean:
	find build -type f -name '*.o' -delete
	rm -fv bin/lib$(PROJECT).a
