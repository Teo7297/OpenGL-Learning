add_library(vertexArray VertexArray.cpp)

target_link_libraries(vertexArray vertexBuffer renderer)

target_include_directories(vertexArray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
