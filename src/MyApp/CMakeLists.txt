add_executable(${PROJECT_NAME} main.cpp appicon.rc)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

target_include_directories(${PROJECT_NAME} PUBLIC 
external/glfw/include
)

if(USE_LOGGER)
    list(APPEND EXTRA_LINKS Logger)
endif()

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    opengl32
    glfw
    glad
    linmath
    glew_s
    renderer
    vertexBuffer
    indexBuffer
    ${EXTRA_LINKS}
)