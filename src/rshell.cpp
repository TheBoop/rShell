#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
typedef tokenizer<char_separator<char> > mytok;

//Takes in commands and returns true or false
bool checkCommand(const char* command ,char* argList[]){
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

    //Take in list of parsed commands
    char* command;//The command
    queue<char*> args;

    //While loop that repeats $ and cin command
    while(input != "exit"){
        cout << "$ ";
        getline(cin,input);//get input
        
//================Tokenize the input into multiple parts==============
        //separators
        char_separator<char> delim(" ","|&;i#");
        tokenizer<char_separator<char> > mytok(input, delim);//seperat the input
    
        for(mytok::iterator it = mytok.begin(); it != mytok.end(); ++it){
            // cout << "token: " << *it << endl; check the it
            // string array to store the temp 
            const string tempArgs[];
            command = 
            if(it == mytok.begin()){
                
            }else{
                commands[commandPos] = *it;
                commandPos++;
            }
        }
            
//==============RUN THE COMMANDS======================
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












    }

    return 0;
}
