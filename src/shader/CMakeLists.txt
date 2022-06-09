add_library(shader Shader.cpp)

target_link_libraries(shader renderer)

target_include_directories(shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
