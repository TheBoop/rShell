#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

//Takes in commands and returns true or false
bool checkCommand( string s){
    
    int pid = fork();
    if(s == "exit"){
        exit(0);
    }
}

int main(int argc,char **argv){
    string command;
    cout << "begin shell" << endl;

    //While loop that repeats $ and cin command
    while(command != "exit"){
        cout << "$ ";
        cin >> command;
    }

    return 0;
}
