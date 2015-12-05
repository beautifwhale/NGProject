# Build script for OSX systems

gcc -dynamiclib hangman.c -L../libsocket/libs -o libs/libhangman.dylib -lsocket
