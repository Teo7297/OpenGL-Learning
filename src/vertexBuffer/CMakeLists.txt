add_library(vertexBuffer VertexBuffer.cpp)

target_link_libraries(vertexBuffer renderer)

target_include_directories(vertexBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
