# Installing Dependencies

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
