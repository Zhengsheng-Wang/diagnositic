cmake_minimum_required(VERSION 3.10)

# your project name
project("diagnostic")

# find includes in the corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# run moc automatically
set(CMAKE_AUTOMOC ON)
# run rcc automatically
set(CMAKE_AUTORCC ON)
# if you're using Widgets and editing .ui forms in Designer
#set(CMAKE_AUTOUIC ON)

message("Looking for Qt...")
# Qt modules (https://doc.qt.io/qt-5/qtmodules.html) you're using in your application
set(CMAKE_PREFIX_PATH "F:/Qt/5.15.0/mingw81_64")

# Qt package.
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

# Python pacakge.
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Resource file.
set(QRC_FILES diagnostic.qrc)

# your source files
set(sources
    main.cpp
    mainwindow.cpp
    #commander.cpp
    diagram.cpp
    element.cpp
    arrow.cpp
)

set(CMAKE_BUILD_TYPE Debug)
# name of the .exe file, window flag and the list of things to compile
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${sources} ${QRC_FILES})

# also tell CMake to link with the libraries from the line 14
# otherwise it'll think that you wanted to find those just for fun and won't link them
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt5::Widgets
    ${PYTHON_LIBRARIES}
    )