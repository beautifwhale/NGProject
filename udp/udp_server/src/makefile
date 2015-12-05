GCC=gcc
LIBS=-lhangman
LIB_PATH=-L/usr/lib
SOURCE_FILES=udp_server.c

all: server

server:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o serverUDP $(LIBS)

clean:
	rm -rf serverUDP
