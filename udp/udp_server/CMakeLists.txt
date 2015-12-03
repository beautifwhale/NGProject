cmake_minimum_required(VERSION 2.8)
project (serverUDP)
include(ExternalProject)

set(libhangman_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libhangman)
set(libhangman_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/libhangman)

ExternalProject_Add(libhangman
   PREFIX ${libhangman_PREFIX}
   GIT_REPOSITORY https://github.com/damorton/libhangman.git

   SOURCE_DIR ${libhangman_SOURCE_DIR}
)


# Include API .h files for libs
#include_directories(${libhangman_SOURCE_DIR}/include)

# Set project libs for linker
# Don't mistake osx for unix
if(UNIX AND NOT APPLE)
  set(PROJECT_LINK_LIBS libhangman.so)
else()
  set(PROJECT_LINK_LIBS libhangman.dylib)
endif()
link_directories(${libhangman_SOURCE_DIR}/lib)
 
# Set sources for target
set(SOURCES src/udp_server.c)

#Build the executable into the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Create the binary file
add_executable(serverUDP ${SOURCES})

# Add dependencies for lib
add_dependencies(serverUDP libhangman)

# Link libhangman
target_link_libraries(serverUDP ${PROJECT_LINK_LIBS} )

# Install the binary file in the bin/ folder
install(TARGETS serverUDP DESTINATION bin)
