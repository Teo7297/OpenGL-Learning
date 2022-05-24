configure_file(ProjectConfig.h.in ProjectConfig.h)

add_subdirectory(MyApp)

option(USE_LOGGER "Basic logging library" ON)
if(USE_LOGGER)
    add_subdirectory(Logger)
endif()

add_subdirectory(glad)
add_subdirectory(Linmath)