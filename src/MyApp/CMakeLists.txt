add_executable(${PROJECT_NAME} main.cpp appicon.rc)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

target_include_directories(${PROJECT_NAME} PUBLIC 
external/glfw/include
)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    opengl32
    glfw
    glm
    glad
    glew_s
    renderer
    vertexBuffer
    indexBuffer
    vertexArray
    shader
    stb_image
    texture
    ${EXTRA_LINKS}
)