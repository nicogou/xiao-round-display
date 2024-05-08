add_subdirectory(ui)

target_include_directories(app PRIVATE ui)

FILE(GLOB display_sources *.c)
target_sources(app PRIVATE ${display_sources})
