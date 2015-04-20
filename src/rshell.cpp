#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
typedef tokenizer<char_separator<char> > mytok;

//Takes in commands and returns true or false
bool checkCommand(const char* command ,char* argList[]){
    //cout << "Check Command" << endl;
    int status, exStat = -1;
    int pid = fork();//forked
    if(pid <= -1){
        perror("Forking Error");
        exit(1);
    }else if (pid == 0){ //Children
        int exStat = execvp(command ,argList);
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

bool shouldRun(const char* command, char* argList[], int seperator, bool previous){
    //seperator 0 = none, 1 = comment, 2 = ||, 3 = &&, 4 = ;
    //cout << "Should Run Check" << endl;
    if(seperator == 0 || seperator == 1){
        //cout << "Case 1" << endl; 
        return false; 
    }else if(seperator == 2 && !previous){
        //cout << "Case 2" << endl; 
        checkCommand(command, argList);
        return true;
    }else if(seperator == 3 && previous){
        //cout << "Case 3" << endl; 
        checkCommand(command, argList);
        return true;
    }else if(seperator == 4){
        //cout << "Case 4" << endl; 
        checkCommand(command, argList);
        return true;
    }
    //cout << "Case 1" << endl; 
    return false;
}

int main(int argc,char **argv){
    //Login



    //Declarations
    bool hasAnd = false, hasOr = false, previous=false;
    string input;
    int argSize = 0, seperator = 0;
    //Take in list of parsed commands
    char* args[5001];//arguments, 1 more for good luck
    
    //While loop that repeats $ and cin command
    while(true){
        cout << "$ ";
        getline(cin,input);//get input
        //cout << input << endl;
//================Tokenize the input into multiple parts==============
        //separators
        char_separator<char> delim(" ","|&;#");
        tokenizer<char_separator<char> > mytok(input, delim);//seperat the input
        bool comment = false;

        for(mytok::iterator it = mytok.begin(); it != mytok.end(); ++it){
            cout << "token: " << *it << "arg: " << argSize << endl; 
            //Check for connectors
            if(*it == "#"||comment){
                cout << "checked #" << endl;
                comment = true;
                seperator = 1;
            }else if(*it == "&"){
                if(hasAnd){
                    cout << "checked &" << endl;
                    hasAnd = false;
                }
                hasAnd = true;
            }else if(*it == "|"){
                if(hasOr){
                    cout << "|" << endl;
                    hasOr = false;
                }
                hasOr = true;
            }else if(*it == ";"){
                cout << ";" << endl;
                seperator = 4;
            }
            else{
            // store the tokenized arrays
                //cout << "STORE" << endl;
                hasOr=false;
                hasAnd=false;
                if(!comment){
                    char* temp = const_cast<char*>(it->c_str());
                    args[argSize] = temp; 
                    argSize++;
                }
            }
            cout << "The Elements: ";
            //Print Check
            for(int i = 0; i < argSize; i++)
                cout << args[i]<< " ";
            cout << "Args: "<<argSize << endl;

            //Clears arg if it runs
            if(shouldRun(args[0],args,seperator,previous)){
                //cout << "CLEAR" << endl;
                for(int i =0; i<argSize;i++)
                    args[i] = '\0';
                seperator = 0;
                argSize = 0;
            }
//==============RUN THE COMMANDS======================
        }
        if(shouldRun(args[0],args,4,previous)){
            for(int i =0; i<argSize;i++)
                    args[i] = '\0';
            argSize = 0;
        }

    }

    return 0;
}
