cmake_minimum_required(VERSION 2.8)
project (clientUDP)
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
set(PROJECT_LINK_LIBS libhangman.dylib)
link_directories(${libhangman_SOURCE_DIR}/lib)
 
# Set sources for target
set(SOURCES src/udp_client.c)

#Build the executable into the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Create the binary file
add_executable(clientUDP ${SOURCES})

# Add dependencies for lib
add_dependencies(clientUDP libhangman)

# Link libhangman
target_link_libraries(clientUDP ${PROJECT_LINK_LIBS} )

# Install the binary file in the bin/ folder
install(TARGETS clientUDP DESTINATION bin)