cmake_minimum_required(VERSION 3.0)
project("feature_toc")

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake magical command for conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# using git version
set(${PROJECT_NAME}_MAJOR_VERSION "1")
set(${PROJECT_NAME}_MINOR_VERSION "1")
include(cmake_modules/version.cmake)

# defining cmake version for installer mainly
set(CMAKE_PROJECT_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${GIT_NUMBER})

# osx icon
set(MACOSX_BUNDLE_ICON_FILE feature_toc.icns)
set(ICON_PATH "${CMAKE_SOURCE_DIR}/art/feature_toc.icns")
set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# remove deprecated warnings on windows
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

INCLUDE_DIRECTORIES(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE WIN32
        src/app.cpp
        src/framemain.cpp
        src/bitmaps.cpp
        src/tocctrl.cpp
        src/tocctrlmodel.cpp
        ${CMAKE_BINARY_DIR}/version.cpp
        ${ICON_PATH}
        src/resource.rc)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        BUNDLE DESTINATION .)

