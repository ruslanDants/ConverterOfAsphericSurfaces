cmake_minimum_required(VERSION 2.8)

project(ConvertAsphereCoefficients)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

find_package(Boost 1.36.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

file (GLOB_RECURSE SOURCES "*.cpp")
file (GLOB_RECURSE INCLUDES "*.h")

qt5_wrap_ui(UIS_HDRS
    mainWindow.ui
    help.ui
)

add_executable(converter ${SOURCES} ${INCLUDES} ${UIS_HDRS})
target_link_libraries(converter Qt5::Core Qt5::Gui)




