GCC=gcc
LIBS=-lsocket
LIB_PATH=-L"usr/lib"
SOURCE_FILES=fork_client.c

all: client

client:
	$(GCC) $(LIB_PATH) $(SOURCE_FILES) -o clientForkTCP $(LIBS)

clean:
	rm -rf clientForkTCP

