add_library(texture Texture.cpp)

target_link_libraries(texture renderer stb_image)

target_include_directories(texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
