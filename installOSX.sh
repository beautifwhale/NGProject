echo "libsocket installing..."
cd libsocket
./buildOSX.sh
echo "UDP client and server building..."
cd ../udp/udp_client/src
make
cd ../../udp_server/src
make
echo "Fork client and server building..."
cd ../../../fork/fork_client/src
make
cd ../../fork_server/src
make
#echo "Select client and server building..."
#cd ../../../select/select_client/src
#make
#cd ../../select_server/src
#make
