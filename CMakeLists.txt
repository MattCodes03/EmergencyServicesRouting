cmake_minimum_required (VERSION 3.12)

project("EmergencyRouting")


file(GLOB_RECURSE SOURCES
    src/*.h
    src/*.cpp
)

set(wxBUILD_SHARED OFF)
set(wxBUILD_USE_STATIC_RUNTIME ON)

add_subdirectory(wxWidgets)
add_subdirectory(SQLiteCpp)

add_executable(EmergencyRouting ${SOURCES})
target_link_libraries(EmergencyRouting wx::net wx::core wx::base wx::gl)
target_link_libraries(EmergencyRouting SQLiteCpp sqlite3)

if(UNIX)
	target_link_libraries(EmergencyRouting X11)
endif()

add_custom_target(ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/src/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
)
add_dependencies(EmergencyRouting ASSETS)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EmergencyRouting PROPERTY CXX_STANDARD 20)
endif()
