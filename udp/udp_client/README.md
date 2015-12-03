# Hangman UDP Client

Hangman is a networked game using a BSD Sockets to communicate between client and server. It is built using libhangman which uses libsocket for network communications.

## Build

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Dependencies

- libhangman  https://github.com/damorton/libhangman.git
- libsocket   https://github.com/damorton/libsocket.git

Libraries used in the project are built and installing using cmake.

