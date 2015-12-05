GCC=gcc
LIBS=-lhangman
LIB_PATH=-L/usr/lib
SOURCE_FILES=fork_server.c

all: server

server:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o serverForkTCP $(LIBS)

clean:
	rm -rf serverForkTCP

