add_library(test TestClearColor.cpp)

target_link_libraries(test renderer imgui)

target_include_directories(texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)