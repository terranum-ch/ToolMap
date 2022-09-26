# CMAKE BUILD FILE FOR TESTING TOOLMAP
# USE AND MYSQL AND WXWIDGETS (CONSOLE)
# PART OF QA FOR TOOLMAP 2
# (c) LUCIEN SCHREIBER CREALP 2009
cmake_minimum_required(VERSION 3.0)

MESSAGE(STATUS "USING UNIT TESTING")

SET(PROGNAMETEST ${CMAKE_PROJECT_NAME}_test)

# Prepare parameters file
SET(UNIT_TESTING_PATH CACHE PATH "Path where the projects for unit testing are located")
IF (NOT UNIT_TESTING_PATH)
    include(FetchContent)
    FetchContent_Declare(
            test-files
            GIT_REPOSITORY https://github.com/terranum-ch/ToolMap-test-files
            GIT_TAG fd74f24ef8366abd26bae7d24b96aebe1d1f5649) # database version 232
    FetchContent_MakeAvailable(test-files)
    set(UNIT_TESTING_PATH "${CMAKE_BINARY_DIR}/_deps/test-files-src")
ENDIF (NOT UNIT_TESTING_PATH)
message(STATUS "Test files location: ${UNIT_TESTING_PATH}")

SET(UNIT_TESTING_USE_PROXY CACHE BOOL "Do we use a proxy for connecting to the web ?")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/test_param.h.in
        "${PROJECT_BINARY_DIR}/test_param.h")
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

# Search all source files
file(GLOB src_tests ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(UnitTests ${src_tests})

target_link_libraries(UnitTests ${CONAN_LIBS} "${PROJECT_NAME}_lib")
add_test(NAME test COMMAND UnitTests)

# suppressing warning for missing PDB files for linked libs
if (MSVC)
    set_target_properties(UnitTests PROPERTIES LINK_FLAGS "/ignore:4099")
endif ()

# Install step for linking *dylib to the executable_path (not a bundle here)
IF (APPLE)
    add_custom_command(
            TARGET UnitTests
            POST_BUILD
            COMMAND install_name_tool -change @rpath/libgdal.30.dylib @executable_path/libgdal.30.dylib bin/UnitTests
            COMMENT "Installing GDAL library for Unit testing"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
ENDIF (APPLE)
