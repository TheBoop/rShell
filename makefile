CPPFLAGS = -std=c++11
CFLAGS = -ansi -pedantic -Wall -Werror

all: rshell

rshell: ; mkdir bin ; g++ $(CPPFLAGS) $(CFLAGS) src/rshell.cpp -o bin/rshell.cpp

clear: ; rm -rf bin/
