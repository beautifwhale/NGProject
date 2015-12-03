# NGProject

NGProject (Networked Games Project) conatins 3 versions of a networked Hangman game. It demonstrates using the libhangman game library to create games using different network protocols and designs. Libhangman uses libsocket for network communications and is installed alongside libhangman during the build stage.

- fork
    - fork client
    - fork server
- select
    - select client
    - select server
- udp
    - udp client
    - udp server

## Build 

You must navigate to the root of each project folder and follow the instructions in the README file to build and install each one.


# Hangman

Hangman is a networked game using a BSD Sockets to communicate between client and server. It is built using libhangman which uses libsocket for network communications.

## Dependencies

- libhangman  https://github.com/damorton/libhangman.git
- libsocket   https://github.com/damorton/libsocket.git

Libraries used in the project are built and installed using cmake.

