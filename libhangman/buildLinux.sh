# Build script for Linux systems

gcc -c -Wall -Werror -fpic game.c
gcc -shared -o libs/libhangman.so game.o -L../libsocket/libs -lsocket
sudo cp libs/libhangman.so /usr/lib
