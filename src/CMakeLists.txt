configure_file(ProjectConfig.h.in ProjectConfig.h)

find_package(OpenGL)

# GLEW
add_library(glew_s STATIC IMPORTED)
set_target_properties(glew_s PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/glew/lib/Release/Win32/glew32s.lib")
set_target_properties(glew_s PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/glew/include")

# Application libraries
add_subdirectory(MyApp)

add_subdirectory(renderer)
add_subdirectory(vertexBuffer)
add_subdirectory(indexBuffer)
add_subdirectory(vertexArray)
add_subdirectory(shader)
add_subdirectory(texture)

add_subdirectory(tests)