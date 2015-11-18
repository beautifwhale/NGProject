cd libsocket
./buildLinux.sh
echo "libsocket install successful"
cd ../udp/udp_client/src
make
cd ../../udp_server/src
make
echo "UDP client and server build successful"
