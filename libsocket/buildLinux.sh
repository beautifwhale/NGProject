# Build script for Linus systems

gcc -c -Wall -Werror -fpic socket.c
gcc -shared -o libs/libsocket.so socket.o
sudo cp libsocket.so /usr/lib
