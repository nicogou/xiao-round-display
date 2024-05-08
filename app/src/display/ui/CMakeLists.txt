add_definitions(-DEEZ_FOR_LVGL -DLV_LVGL_H_INCLUDE_SIMPLE -DLV_CONF_INCLUDE_SIMPLE)

include_directories(../../../../../modules/lib/gui/lvgl)

add_subdirectory(eez-framework)

target_link_libraries(app PRIVATE
    eez-framework
)

FILE(GLOB ui_sources *.c)
target_sources(app PRIVATE ${ui_sources})
