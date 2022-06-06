add_library(indexBuffer IndexBuffer.cpp)

target_link_libraries(indexBuffer renderer)

target_include_directories(indexBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
