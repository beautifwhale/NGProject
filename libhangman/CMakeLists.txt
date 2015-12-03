cmake_minimum_required(VERSION 2.8.9)
project(libhangman)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(../libsocket)

set( PROJECT_LINK_LIBS socket )
link_directories(../libsocket/lib)

include_directories(../libsocket/include)

set(SOURCES "hangman.c")

add_library(hangman SHARED ${SOURCES})
target_link_libraries(hangman ${PROJECT_LINK_LIBS} )

install(TARGETS hangman DESTINATION lib)
install(FILES hangman.h DESTINATION include)
