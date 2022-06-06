add_library(vertexBuffer VertexBuffer.cpp VertexBufferLayout.cpp)

target_link_libraries(vertexBuffer renderer glew_s)

target_include_directories(vertexBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
