CFLAGS = -ansi -pedantic -Wall -Werror

all: rshell

rshell: rshell.cpp ; g++ $(CFLAGS) src/rshell.cpp -o bin/rshell

clear ; rm -rf bin/
