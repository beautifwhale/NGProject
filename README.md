# Build Ubuntu

IMPORTANT:

First install libsocket on system by copying the library into /usr/lib and changing the permissions if needed.

$ cd ngproject/libsocket/
$ cp libsocket.so /usr/lib
$ chmod 0755 /usr/lib/libsocket.so

Now update the ld cache for the linker

$ sudo ldconfig

Now build each version of the project.

======================================

fork client/server:

$ cd ngproject/fork/<version>
$ make


Building the other projects will be similar when complete. 
