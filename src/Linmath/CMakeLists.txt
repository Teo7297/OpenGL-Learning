add_library(linmath linmath.cpp)

target_include_directories(linmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)