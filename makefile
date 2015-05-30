GCC = g++
CPPFLAGS = -std=c++11
CFLAGS = -ansi -pedantic -Wall -Werror -g
FILEPATHRSHELL = ./src/rshell.cpp
FILEPATHLS = ./src/ls.cpp
OUTFILERSHELL = bin/rshell
OUTFILELS = bin/ls

all: rshell

rshell:
	if [ ! -d bin ]; then mkdir bin; fi
	$(GCC) $(CPPFLAGS) $(CFLAGS) $(FILEPATHRSHELL) \
	-o $(OUTFILERSHELL)

ls:
	if [ ! -d bin ]; then mkdir bin; fi
	$(GCC) $(CPPFLAGS) $(CFLAGS) $(FILEPATHLS) \
	-o $(OUTFILELS)

lsd:
	if [ ! -d bin ]; then mkdir bin; fi
	$(GCC) -g $(CPPFLAGS) $(CFLAGS) $(FILEPATHLS) \
	-o bin/lsd

run:
	bin/rshell

runls:
	$(OUTFILELS) -R -l -a ./makefile

clean:
	rm -rf bin/
