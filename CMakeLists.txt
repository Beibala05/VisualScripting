cmake_minimum_required(VERSION 3.10)

project(Nodes VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
 
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(src)

set(SOURCES
    src/main.cpp
    src/main_window.cpp
    src/node.cpp
    src/viewport.cpp
    src/functional.cpp
)

set(HEADERS
    src/main_window.h
    src/node.h
    src/viewport.h
    src/functional.h
    src/style.h
    src/cross.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
