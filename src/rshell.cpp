#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//Takes in commands and returns true or false
bool checkCommand(const char* comList ,char* argList[]){
    int status, exStat = -1;
    int pid = fork();
    if(pid <= -1){
        perror("Forking Error");
        exit(1);
    }else if (pid == 0){ //Children
        int exStat = execvp(comList ,argList);
        if(exStat == -1){
            perror("Executable or arg Error");
            exit(1);
        }
    }else if (pid > 0) {//Parent
        if(waitpid(pid, &status, 0) == -1)
            perror("Wait Error");
    }
    return exStat;
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
