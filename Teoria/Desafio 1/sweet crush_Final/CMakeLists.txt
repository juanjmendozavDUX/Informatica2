cmake_minimum_required(VERSION 3.16)

project(SweetCrush LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(SweetCrush
    menu.cpp
    funciones.cpp
    logica.h
)

target_link_libraries(SweetCrush PRIVATE Qt${QT_VERSION_MAJOR}::Core)
