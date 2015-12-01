# Build script for OSX systems

gcc -dynamiclib game.c -L../libsocket/libs -o libs/libhangman.dylib -lsocket
sudo cp libs/libhangman.dylib /usr/lib
