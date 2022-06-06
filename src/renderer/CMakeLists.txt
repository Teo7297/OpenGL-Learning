add_library(renderer Renderer.cpp)

target_link_libraries(renderer glew_s)

target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
