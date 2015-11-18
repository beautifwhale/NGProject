GCC=gcc
LIBS=-lsocket
LIB_PATH=-L"usr/lib"
SOURCE_FILES=udp_client.c

all: client

client:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o clientUDP $(LIBS)

clean:
	rm -rf clientUDP

