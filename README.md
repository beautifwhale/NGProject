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

## Hangman

Hangman is a networked game using a BSD Sockets to communicate between client and server. It is built using libhangman which uses libsocket for network communications.

## Building Dependencies

libhangman https://github.com/damorton/libhangman.git
libsocket https://github.com/damorton/libsocket.git

## Linux

```
$ ./install.sh
```

## OSX

```
$ ./installOSX.sh
```

This will build the `libsocket` and `libhangman` libraries used in the project. All of the project binaries will also be built and linked against libsocket and libhangman. The libs will be installed to the `/usr/lib` folder on the system.

## Running the project

For each version of Hangman game navigate to the `src/` folder of the server first and run using `./`. Then navigate to the corresponding client `src/` directory and run using `./`. Clients may need additional arguments such as `hostname` and/or `service-name`.

```
$cd ngproject/udp/udp_server/src/
$./serverUDP
```
