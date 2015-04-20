# rShell
##Description
Simple shell designed for unix. Includes only basic functions such as:
* ls
* echo
* mkdir
Also connectors such as:
* ||
* && 
* ;
and supports functions.

##Requirements
Only major requirement is the Boost library for C++
also the SDL libraries and a c++ compiler;

##Bugs and Limitations
*Throws errors when not given a proper executable
*Throws errors when not given proper arguments
*Throws errors when given odd number of | and &
*ls does not work by itself, but works with argument inputs
*cd command does not work
*no command history
*Host name can only be 200 characters long
