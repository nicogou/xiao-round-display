add_definitions(-DEEZ_FOR_LVGL -DLV_LVGL_H_INCLUDE_SIMPLE)

add_subdirectory(eez-framework)

target_link_libraries(app PRIVATE
	lvgl
    eez-framework
)

FILE(GLOB ui_sources *.c)
target_sources(app PRIVATE ${ui_sources})
