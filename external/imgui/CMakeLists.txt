add_library(imgui 
    imgui.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
)

target_link_libraries(imgui glfw)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
