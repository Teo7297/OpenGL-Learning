add_executable(${PROJECT_NAME} main.cpp appicon.rc)

find_package(OpenGL)

if(USE_LOGGER)
    list(APPEND EXTRA_LINKS Logger)
endif()

# GLEW
add_library(glew_s STATIC IMPORTED)
set_target_properties(glew_s PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/glew/lib/Release/Win32/glew32s.lib")
set_target_properties(glew_s PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/glew/include")


include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

target_include_directories(${PROJECT_NAME} PUBLIC 
    external/glfw/include
)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    opengl32
    glfw
    glad
    linmath
    glew_s
    ${EXTRA_LINKS}
)