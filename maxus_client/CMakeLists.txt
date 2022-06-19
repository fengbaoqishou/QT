cmake_minimum_required(VERSION 3.1.0)

option(WIN-MSVC-X86_64 "option for Windows Platform" OFF)
option(LINUX-GCC-X86_64 "option for Linux Platform" OFF)

project(MonsterClient)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "/home/lijiajun/Qt5.12.9/5.12.9/gcc_64/lib/cmake/Qt5")


set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)

find_package(OpenCV REQUIRED) 
include_directories( ${OpenCV_INCLUDE_DIRS} ) 

#find_package(ffmpeg REQUIRED)
#include_directories(${ffmpeg_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}/third_party/linux-gcc-x86_64/lcm/include"
			"/usr/local/include"
			"/usr/include/x86_64-linux-gnu"
            "/usr/local/include")
link_directories("${CMAKE_SOURCE_DIR}/third_party/linux-gcc-x86_64/lcm/lib"
			"/usr/local/lib"
			"/usr/include/x86_64-linux-gnu"
            "/usr/local/lib")

add_executable(monster_client
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    lcmthread.cpp
    plotwindow.cpp
    qcustomplot.cpp
    apa_client.cpp
    p2p_client.cpp
    vpld_client.cpp
    vdr_client.cpp
    freespacewindow.cpp
    freespacewindow.ui
    sonarcostmapwindow.cpp
    sonarcostmapwindow.ui
)

target_link_libraries(monster_client Qt5::Widgets Qt5::Core Qt5::Gui  Qt5::PrintSupport lcm avcodec avutil swscale ${OpenCV_LIBS} libzmq.so)


# install(TARGETS monster_client
# RUNTIME DESTINATION bin)


# install(CODE "add_custom_command(TARGET monster_client POST_BUILD COMMAND dir)" )
# install(SCRIPT "${CMAKE_SOURCE_DIR}/script/windeployqt.bat")
# add_custom_command(
#     TARGET monster_client
#     POST_BUILD
#     COMMAND "${CMAKE_SOURCE_DIR}/script/windeployqt.bat"
#     COMMENT "**********copy all lib**********"
#     )
