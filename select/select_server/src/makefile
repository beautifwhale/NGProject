GCC=gcc
LIBS=-lsocket
LIB_PATH=-L"usr/lib"
SOURCE_FILES=hangserver_iter.c game.c

all: server

server:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o serverSelectTCP $(LIBS)

clean:
	rm -rf serverSelectTCP
