# Define the plugin to compile
set(TARGET_PLUGIN "JPLog2")

# Include necessary directories
include_directories(${NUKE_INCLUDE_DIRS})

# Create the selected plugin
add_library(${TARGET_PLUGIN} MODULE JPLog2.cpp)
add_library(NukePlugins::${TARGET_PLUGIN} ALIAS ${TARGET_PLUGIN})
target_link_libraries(${TARGET_PLUGIN} PRIVATE ${NUKE_DDIMAGE_LIBRARY})

if (NUKE_VERSION_MAJOR VERSION_GREATER_EQUAL 14.0)
    target_compile_definitions(DespillAP PRIVATE NOMINMAX _USE_MATH_DEFINES)
endif()

# Plugin installation
install(TARGETS ${TARGET_PLUGIN} DESTINATION .)