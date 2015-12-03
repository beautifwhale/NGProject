cmake_minimum_required(VERSION 2.8.9)
project(libsocket)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_BUILD_TYPE Release)

set(SOURCES socket.c)

add_library(socket SHARED ${SOURCES})

install(TARGETS socket DESTINATION lib)
install(FILES socket.h DESTINATION include)

