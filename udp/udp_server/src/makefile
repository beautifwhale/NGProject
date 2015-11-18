GCC=gcc
LIBS=-lsocket
LIB_PATH=-L"usr/lib"
SOURCE_FILES=udp_server.c game.c

all: server

server:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o serverUDP $(LIBS)

clean:
	rm -rf serverUDP
