#Installation
first clone file from git and move the file to desired location
rShell
* open the file and runL 'make rshell' to compile
* run 'bin/rShell' to execute
ls
* open the file and run: 'make ls'
* run 'bin/ls' to execute

## rShell
###Description
Simple shell designed for unix. Includes only basic functions such as:
* ls
* echo
* mkdir

Also connectors such as:
* ||
* && 
* ;
and supports functions.

## ls
###Description
ls command for bash. Accepts parameters -l, -R & -a ONLY. Accepts any 
valid paths. See limitations below for indepth details.

##Requirements
Only major requirement is the Boost library for C++
also the SDL libraries and a c++ compiler;

##Bugs and Limitations
ls
* After given invalid path or parameters, program will stop
* When show files or directories with large names, file will be out of alignment
* The total count for -l parameter is shown at the bottom of the list instead of the top

rShell
* Throws errors when not given a proper executable
* Throws errors when not given proper arguments
* Throws errors when given odd number of | and &
* ls does not work by itself, but works with argument inputs
* cd command does not work
* no command history
* Host name can only be 200 characters long
