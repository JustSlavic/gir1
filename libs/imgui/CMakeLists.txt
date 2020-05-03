project(imgui)

set(CMAKE_CXX_STANDARD 14)

set(HEADER_FILES
        imconfig.h
        imgui.h
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        imgui_impl_glfw.h
        imgui_impl_opengl3.h)

set(SOURCE_FILES
        imgui.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_impl_glfw.cpp
        imgui_impl_opengl3.cpp)
#        imgui_demo.cpp

add_library(imgui STATIC ${SOURCE_FILES})
