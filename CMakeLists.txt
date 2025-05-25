# CMAKE BUILD FILE FOR ToolMap 2
# (c) TERRANUM 2022
cmake_minimum_required(VERSION 3.10)

PROJECT("ToolMap")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build/cmake ${CMAKE_MODULE_PATH})
ADD_DEFINITIONS(-DEMBEDDED_LIBRARY)

# using git version
set(${PROJECT_NAME}_MAJOR_VERSION "4")
set(${PROJECT_NAME}_MINOR_VERSION "0")
set(CODE_NAME "NUGLAR")
include(version)

# defining cmake version for installer mainly
set(CMAKE_PROJECT_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${GIT_NUMBER})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (UNIX AND NOT APPLE)
    find_package(PNG REQUIRED)
    message(STATUS "PNG_LIBRARIES: ${PNG_LIBRARY}")
endif ()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)
if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

# Export for sourcetrail
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# disable CRT warnings on Windows
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

## Code coverage for IDE, see https://www.jetbrains.com/help/clion/code-coverage-clion.html#compiler-flags
SET(USE_CODECOVERAGE_IDE OFF CACHE BOOL "Should we do code coverage for IDE ?")
if (USE_CODECOVERAGE_IDE)
    message(STATUS "Code coverage for IDE")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif (USE_CODECOVERAGE_IDE)

# Enable tests
SET(USE_UNITTEST CACHE BOOL "Should we create the tests for ${CMAKE_PROJECT_NAME}")

# Enable code coverage
if (USE_UNITTEST AND CMAKE_COMPILER_IS_GNUCXX)
    option(USE_CODECOV "Sould we do code coverage with lcov ?" OFF)
else (CMAKE_COMPILER_IS_GNUCXX)
    set(USE_CODECOV OFF)
endif ()

## External projects
include(ExternalProject)
set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/external)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${EXTERNAL_DIR}")
include_directories(${EXTERNAL_DIR}/include)
link_directories(${EXTERNAL_DIR}/lib)
link_directories(${EXTERNAL_DIR}/lib64)

## Option to get ToolBasView
set(WITH_TOOLBASVIEW OFF CACHE BOOL "Should we build ToolBasView along with ToolMap?")

## Add ToolBasView as external project
if (WITH_TOOLBASVIEW)
    ExternalProject_Add(ToolBasView
            GIT_REPOSITORY https://github.com/terranum-ch/ToolBasView
            GIT_TAG v1.4.225
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND conan install ../ToolBasView -s build_type=Release --build=missing
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR} -DCMAKE_BUILD_TYPE=Release
            BUILD_COMMAND conan build ../ToolBasView
            INSTALL_COMMAND ""
    )
endif ()

# Sources and headers 
FILE(GLOB_RECURSE SRC_FILES src/*.cpp)
SET(MY_SEARCHED_FILE "src/core/toolmap.cpp")
FILE(GLOB MAIN_SRC_FILE "${MY_SEARCHED_FILE}")
LIST(LENGTH SRC_FILES MYLENGTH)
LIST(FIND SRC_FILES "${MAIN_SRC_FILE}" MYFOUND)
IF (MYFOUND EQUAL -1)
    MESSAGE(FATAL_ERROR "Following file not found : ${MY_SEARCHED_FILE}")
ENDIF (MYFOUND EQUAL -1)
LIST(REMOVE_AT SRC_FILES ${MYFOUND})
FILE(GLOB_RECURSE HEAD_FILES "src/*.h")
SOURCE_GROUP(header FILES ${HEAD_FILES})


# Includes
INCLUDE("${PROJECT_SOURCE_DIR}/src/components/wxtreemultictrl/wxtreectrloptions.cmake")
# INCLUDE("plugins/lsversion/build/use_lsversion.cmake")
# INCLUDE("plugins/lscrashreport/build/use_lscrashreport.cmake")

#INCLUDE_DIRECTORIES(plugins/lscrashreport/src)
#ADD_SUBDIRECTORY(plugins/lscrashreport)

if (UNIX AND NOT APPLE)
    message(STATUS "PNG lib is : ${PNG_LIBRARY}")
endif (UNIX AND NOT APPLE)

# osx icon
set(MACOSX_BUNDLE_ICON_FILE art/toolmap.icns)
set(ICON_PATH "${CMAKE_SOURCE_DIR}/art/toolmap.icns")
set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# Create a library with all sources files mainly for unit testing
ADD_LIBRARY("${CMAKE_PROJECT_NAME}_lib" STATIC ${SRC_FILES})
target_link_libraries("${CMAKE_PROJECT_NAME}_lib" ${CONAN_LIBS})
if (UNIX AND NOT APPLE)
    target_link_libraries("${CMAKE_PROJECT_NAME}_lib" ${PNG_LIBRARY} xkbcommon fontconfig)
endif (UNIX AND NOT APPLE)

# Create the Toolmap binary
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
        MACOSX_BUNDLE WIN32
        ${MAIN_SRC_FILE}
        ${ICON_PATH}
        src/gui/resource.rc
)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
if (UNIX AND NOT APPLE)
    message(STATUS "PNG lib is : ${PNG_LIBRARY}")
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PNG_LIBRARY} xkbcommon fontconfig)
endif (UNIX AND NOT APPLE)

#IF (WIN32)
#    SOURCE_GROUP(resource FILES src/resource.rc)
#    ADD_EXECUTABLE("${CMAKE_PROJECT_NAME}" WIN32 ${MAIN_SRC_FILE} src/gui/resource.rc)
#    TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}" ${WXWINDOWS_LIBS} "${CMAKE_PROJECT_NAME}_lib")
#ELSE (WIN32)
#    ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${MAIN_SRC_FILE} ${HEAD_FILES})
#    IF (APPLE)
#        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}_lib")
#    ELSE (APPLE)
#        #LINUX
#        TARGET_LINK_LIBRARIES(${LSVERSION_NAME} fontconfig)
#        TARGET_LINK_LIBRARIES(${LSCRASHREPORT_NAME} fontconfig)
#        TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" fontconfig)
#        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}_lib" fontconfig)
#    ENDIF (APPLE)
#ENDIF (WIN32)

if (UNIX)
    add_definitions(-std=c++14)
endif (UNIX)

# IF (APPLE)
# Copy plist to ToolMap (should be after add executable!
configure_file(build/custominfo.plist ${CMAKE_BINARY_DIR}/custominfo.plist)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/custominfo.plist)
# ENDIF (APPLE)

# adding multi-core support for Visual Studio Compiler
IF (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    MESSAGE(STATUS "Adding multiprocessor support for Visual Studio (/MP Flag)")
ENDIF (MSVC)

if (USE_CODECOV)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    setup_target_for_coverage_lcov(
            NAME code-coverage
            EXECUTABLE UnitTests
            EXCLUDE "/usr/*" "test/*" "_deps/*" "bin/*" "*/lib/*" "*/include/*" "*/.conan/*" "${PROJECT_SOURCE_DIR}/test/src/*" "${CMAKE_BINARY_DIR}/_deps/*"
            DEPENDENCIES UnitTests)
endif (USE_CODECOV)

## Unit tests
IF (USE_UNITTEST)
    ADD_SUBDIRECTORY(test)
ENDIF (USE_UNITTEST)

# Install step for copying *dylib into application bundle on OSX and for changing their reference from
# absolute to relative
IF (APPLE)
    # change cmake_install_prefix from /usr/local to the binary directory
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
    set(APPS ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_PROJECT_NAME}.app)  # paths to executables
    set(DIRS ${CMAKE_INSTALL_PREFIX}/bin/)  # paths to executables
    INSTALL(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)
ENDIF (APPLE)

# create installer using CPACK
include("build/cpack/cpack_installer.cmake")

