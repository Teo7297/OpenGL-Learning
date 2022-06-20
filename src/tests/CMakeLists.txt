add_library(test Test.cpp TestClearColor.cpp TestTexture2D.cpp)

target_link_libraries(test 
    renderer 
    imgui
    vertexBuffer
    indexBuffer
    vertexArray
    glm
    texture
    shader
)

target_include_directories(texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)