#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <algorithm>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
typedef tokenizer<char_separator<char> > mytok;

//Takes in commands and returns true or false
bool checkCommand(vector<string> argList){
    vector<char *> arguments(argList.size() +1);

    for(size_t i = 0; i < argList.size(); ++i){
        arguments[i] = &argList[i][0];
    }

    int status;
    int pid = fork();//forked const_cast<char*>(argList[0].c_str())
    if(pid < 0){
        perror("Forking Error");
        exit(1);
    }else if (pid == 0){ //Children
        if(execvp(arguments[0],arguments.data())<0){
            perror("Executable or arg Error");
            return false;
        }else{
            cout << "true" << endl;
            return true;
        }
        /*for(unsigned int i = 0; i<argList.size(); i++)
            cout <<  arguments[i] << " ";
        cout << endl;*/
    }else if (pid > 0) {//Parent
        if(waitpid(pid, &status, 0) == -1)
            perror("Wait Error");

        return true;
    }
    //cout << "ExStat: " << exStat << endl;
    return false;
}

bool shouldRun(vector<string> argList, int seperator, bool &previous){
    //seperator 0 = none, 1 = comment, 2 = ||, 3 = &&, 4 = ;
    //cout << "Previous: "<< previous  << endl;
    if(argList.empty()){
        return false;
    }else if(seperator == 0 || seperator == 1){
        //cout << "Case 1" << endl; 
        return false; 
    }else if(seperator == 2 && !previous){
        //cout << "Case 2" << endl; 
        previous = checkCommand(argList);
        return true;
    }else if(seperator == 3 && previous){
        //cout << "Case 3" << endl; 
        previous = checkCommand(argList);
        return true;
    }else if(seperator == 4){
        //cout << "Case 4" << endl; 
        previous = checkCommand(argList);
        return true;
    }
    //cout << "Case 1" << endl; 
    return false;
}

int main(int argc,char **argv){
    //Login
    char host[200];//allocate memeory for host
    char* user;
    int hStat = gethostname(host, 200);
    if(hStat !=0){
        perror("Cannot get Host Name");
    }
    if(getlogin() != NULL){
        user = getlogin();
    }else{
        perror("Cannot get User Name");
    }

    //Declarations
    bool hasAnd = false, hasOr = false, previous=false;
    string input;
    int seperator = 0;
    //Take in list of parsed commands
    //char* args[5001];//arguments, 1 more for good luck
    vector<string> args;

    //While loop that repeats $ and cin command
    while(true){
        cout << user << "@" << host << " $ ";
        cout.flush();//flush just to be safe

        getline(cin,input);//get input
//================Tokenize the input into multiple parts==============
        //separators
        char_separator<char> delim(" ","|&;#");
        tokenizer<char_separator<char> > mytok(input, delim);//seperat the input
        bool comment = false;

        for(mytok::iterator it = mytok.begin(); it != mytok.end(); ++it){
            //cout << "token: " << *it  << endl; 
            //Check for connectors
            if(*it == "exit"){
                exit(0);
            }if(*it == "#"||comment){
                //cout << "checked #" << endl;
                comment = true;
                seperator = 1;
            }else if(*it == "&"){
                if(hasAnd){
                    //cout << "checked &&" << endl;
                    seperator = 3;
                    hasAnd = false;
                }
                hasAnd = true;
            }else if(*it == "|"){
                if(hasOr){
                    //cout << "checked ||" << endl;
                    seperator = 2;
                    hasOr = false;
                }
                hasOr = true;
            }else if(*it == ";"){
                //cout << ";" << endl;
                seperator = 4;
            }
            else{
            // store the tokenized arrays
                //cout << "STORE" << endl;
                hasOr=false;
                hasAnd=false;
                args.push_back(*it);
                seperator = 0;
            }
            //cout << "The Elements: ";
            //Print Check
            //for(unsigned int i = 0; i < args.size(); i++)
            //    cout << args[i]<< " ";
            //cout << " Args: "<<args.size()<< endl;

            //Clears arg if it runs
            if(shouldRun(args,seperator,previous)){
            //cout << "CLEAR1" << endl;
                args.clear();
            }
//==============RUN THE COMMANDS======================
        }
        if(shouldRun(args,4,previous)){
            //cout << "CLEAR2" << endl;
            args.clear();
            previous = false;
        }

    }

    return 0;
}
