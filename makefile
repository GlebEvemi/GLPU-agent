CC = gcc

SRC = src/service.c src/api.c src/config.c src/cJSON.c

OUT = GLPU-Agent.exe

# include paths
INCLUDES = -Iinc -IC:/Users/lee/Downloads/curl-8.19.0_5-win64-mingw/include

# library paths
LIBS = -LC:/Users/lee/Downloads/curl-8.19.0_5-win64-mingw/lib

# linked libraries
LINK = -lcurl -ladvapi32 -lwinmm -lcrypt32

all: test
	$(CC) $(SRC) -o $(OUT) $(INCLUDES) $(LIBS) $(LINK)

# Run unit tests
test:
	@cd tests && make test

# Clean build
clean:
	del $(OUT)
	@cd tests && make test_clean

.PHONY: all clean test 