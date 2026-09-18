cmake_minimum_required(VERSION 3.16)

project(sweet_crush_v2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sweet_crush_v2 main.cpp
    funciones_jugador.h
    funciones_jugador.cpp)

include(GNUInstallDirs)
install(TARGETS sweet_crush_v2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
