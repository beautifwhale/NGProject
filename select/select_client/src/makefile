GCC=gcc
LIBS=-lsocket
LIB_PATH=-L"/usr/lib"
SOURCE_FILES=select_client.c

all: client

client:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o clientSelectTCP $(LIBS)

clean:
	rm -rf clientSelectTCP

