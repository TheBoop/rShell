#include <algorithm>
#include <boost/tokenizer.hpp>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace boost;
typedef tokenizer<char_separator<char> > mytok;

void sigHandler(int sig){
    if(sig == SIGINT){
        cout << endl;
    }
}

//checks if proper files are there for input output
bool properInput(vector<string> argList){
    for(unsigned int i = 0; i<argList.size();++i){
        if(argList[i] == ">"){
            if(i+1>=argList.size()){
                cout<< ">: expect output file" << endl;
                return false;
            }
        }else if(argList[i] == ">>"){
            if(i+1>=argList.size()){
                cout<< ">>: expect output file" << endl;
                return false;
            }
        }else if(argList[i] == "<"){
            if(i+1>=argList.size()){
                cout<< "<: expect input file" << endl;
                return false;
            }
        }else if(argList[i] == "||"){
            if(i+1>=argList.size()){
                cout<< "||: expect input file" << endl;
                return false;
            }
        }
    }
    return true;
}

//Returns the position of operators and fixes arguments
//Also Checks if all the inputs are correct
void concatArg(vector<string> &argList){
    for(unsigned int i = 1; i<argList.size();++i){
        if(argList[i] == ">" && argList[i-1] == ">"){
            argList[i-1] = ">>";
            argList.erase(argList.begin()+i);
            --i;
        }else if(argList[i] == "<" && argList[i-1] == "<"){
            argList[i-1] = "<<";
            argList.erase(argList.begin()+i);
            --i;
        }else if(argList[i] == "<<" && argList[i-1] == "<"){
            argList[i-1] = "<<<";
            argList.erase(argList.begin()+i);
            --i;
        }else if(argList[i] == "|" && argList[i-1] == "|"){
            argList[i-1] = "||";
            argList.erase(argList.begin()+i);
            --i;
        }else if(argList[i] == ">" && argList[i-1] == ">"){
            argList[i-1] = ">>";
            argList.erase(argList.begin()+i);
            --i;
        }else if(argList[i]=="&" && argList[i-1] == "&"){
            argList[i-1] = "&&";
            argList.erase(argList.begin()+i);
            --i;
        }
    }
}
//============Management for the single part of args===============
bool singleCommand(vector<string> argList){
    vector<string> singleArg;
    int fd0,fd1,status;
    //Where the string get's parsed 
    int pid = fork();
    if(pid < 0){
        perror("Forking Error");
        exit(1);
    }else if (pid == 0){
        for(unsigned int i = 0; i<argList.size();++i){
            if(argList[i] == ">"){
                ++i;
                if((fd1=open(argList[i].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 
                    S_IRUSR | S_IWUSR))<0){
                    perror("open fd0");
                    exit(1);
                }
                if(dup2(fd1,1)<0){
                    perror("dup fd0");
                    exit(1);
                }
                if(close(fd1)<0){
                    perror("close fd0");
                    exit(1);
                }
            }else if(argList[i] == ">>"){
                ++i;
                if((fd1=open(argList[i].c_str(),O_WRONLY | O_CREAT | O_APPEND, 
                    S_IRUSR | S_IWUSR))<0){
                    perror("open fd0");
                    exit(1);
                }
                if(dup2(fd1,1)<0){
                    perror("dup fd0");
                    exit(1);
                }
                if(close(fd1)<0){
                    perror("close fd0");
                    exit(1);
                }
            }else if(argList[i] == "<"){
                ++i;
                if((fd0=open(argList[i].c_str(),O_RDONLY))<0){
                    perror("open fd1");
                    exit(1);
                }
                if(dup2(fd0,0)<0){
                    perror("dup fd1");
                    exit(1);
                }
                if(close(fd0)<0){
                    perror("close fd1");
                    exit(1);
                }
            }else{
                singleArg.push_back(argList[i]);
            }
        }
        vector<char *> arguments;
        for(size_t i = 0; i < singleArg.size(); ++i){
            arguments.push_back(&singleArg[i][0]);
        }
        arguments.push_back('\0');
        if(execvp(arguments[0],arguments.data())<0){
            perror("Executable");
            exit(1);
        }
    }else if (pid > 0) {//Parent
        int wpid;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (wpid == -1 && errno == EINTR);
        if (wpid == -1)
        {
            perror("wait error");
        }
    }
    return status;
}

//This is for handeling CD
bool comCD(vector<string> args){
    string oldPwd, newPwd;
    //Changes current directory to home
    if(args.size()==1){
        newPwd = getenv("HOME");
        if(newPwd == "\0"){
            perror("getenv");
            return false;
        }else{
            if(chdir(newPwd.c_str()) < 0){
                perror("chdir");
                return false;
            }
            oldPwd = getenv("PWD");
            if(oldPwd == "\0"){
                perror("old getenv");
                return false;
            }
            if(setenv("OLDPWD", oldPwd.c_str(), 1)<0){
                perror("setenv");
                return false;
            }
            if(setenv("PWD", newPwd.c_str(), 1)<0){
                perror("setenv");
               return false;
            }
        }
    //Changes to preivous directory
    }else if(args[1] == "-"){
        newPwd = getenv("OLDPWD");
        oldPwd = getenv("PWD");
        if(newPwd == "\0"){
            perror("getenv");
            return false;
        }else if(oldPwd == "\0"){
            perror("old getenv");
            return false;
        }else{
            if(chdir(newPwd.c_str()) < 0){
                perror("chdir");
                return false;
            }
            if(setenv("OLDPWD", oldPwd.c_str(), 1)<0){
                perror("setenv");
                return false;
            }
            if(setenv("PWD", newPwd.c_str(), 1)<0){
                perror("setenv");
                return false;
            }
        }
    //Changes to the path
    }else{
        oldPwd = getenv("PWD");
        newPwd = args[1];
        
        if(newPwd == "\0"){
            perror("getenv");
            return false;
        }else if(oldPwd == "\0"){
            perror("old getenv");
            return false;
        }else{
            //Checks for ~
            if(newPwd[0] == '~'){
                string home = getenv("HOME");
                if(home == "\0"){
                    perror("getenv");
                    return false;
                }
                newPwd.replace(0,1,home);
            }
            
            if(chdir(newPwd.c_str()) < 0){
                perror("chdir");
                return false;
            }
            if(setenv("OLDPWD", oldPwd.c_str(), 1)<0){
                perror("setenv");
                return false;
            }
            if(setenv("PWD", newPwd.c_str(), 1)<0){
                perror("setenv");
                return false;
            }
        }
    }
    return true;
}

//============Main management for the arguments===============
void allCommand(vector<string> argList){
    int pipeNum = 0, status;
    vector<string> args;
    vector<vector<string> > commands;
    for(unsigned int i = 0; i<argList.size();++i){
        if(argList[i]=="|"){
            commands.push_back(args);
            args.clear();
            pipeNum++;
        }else{
            args.push_back(argList[i]);
        }
    }
    commands.push_back(args);
    //For if there is no pipes, currently working
    if(pipeNum == 0){
        singleCommand(argList);
        return;
    }

//+++++++++++++++++++++++++=BROKEN++++++++++++++++++++++++++++++   
//=========================PIPES=================================
    pid_t* pids = new pid_t[pipeNum];
    pid_t* forks = new pid_t[pipeNum+1];
    int* pidfd = new int[pipeNum*2]; 
    
    //Set up pipes
    for( int j=0; j < pipeNum; ++j){
        pipe(pidfd + (j*2));
    }
    for( int i=0; i < pipeNum + 1; ++i){
        //cout << "i: " << i << endl;
        if ((forks[i] = fork()) < 0) {
            perror("fork");
            return;
        } else if (forks[i] == 0) {
        //Where to run runargs();
            if(i==0){//-------------------------------------First 
                //cout << "First Fork" << endl;
                //Redirect cout to first pipe
                if(dup2(pidfd[1],1)<0){
                    perror("dup2");
                    return;
                }
                singleCommand(commands[i]);
                //close all the pipes
                for(int j = 0;j<pipeNum*2;++j){
                    if(j != 1){
                        if(close(pidfd[j])<0){
                            perror("close");
                        }
                    }
                } 
            }else if(i==pipeNum){//---------------------Last Fork
                //cout << "Last Fork" << endl;
                //Redirect cin to last pipe
                if(dup2(pidfd[(i-1)*2],0)<0){
                    perror("dup2");
                    return;
                }
                // singleCommand(commands[i]);
                // for(int j = 0;j<pipeNum*2;++j){
                //     close(pidfd[j]);
                // } 
            }else{//----------------------------------------Other Fork
                //cout << " Fork" << endl;
                //Redirect pipes in the mid
                if(dup2(pidfd[(i-1)*2],0)<0){
                    perror("dup2");
                    return;
                }
                
                //Changes cin and cout
                if(dup2(pidfd[(i-1)*2 + 3],1)<0){
                    perror("dup2");
                    return;
                }
                singleCommand(commands[i]);
                for(int j = 0;j<pipeNum*2;++j){
                    close(pidfd[j]);
                } 
            }
            singleCommand(commands[i]);
            for(int j = 0;j < pipeNum*2;++j){
                cout << "child closed: " << j << endl;
                close(pidfd[j]);
            } 
            exit(0);
        }
    }
    
    for(int j = 0;j<pipeNum*2;++j){
        cout << "parent closed: " << j << endl;
        close(pidfd[j]);
    } 
    
    pid_t pid;
    //waits for all the child
    for( int w=0; w < pipeNum+1; ++w) {
        sigignore(SIGINT);
        if((pid = wait(&status))<0){
            perror("wait");
            return;
        }
        cout << "Child PID: "<< pid << endl;
    }
    
    //De-allocate memory
    delete [] pids;
    delete [] pidfd;
    delete [] forks;
    forks = 0;
    pids = 0;
    pidfd = 0;
    
}

//=========================MAIN FUNCTION=======================
int main(int argc,char **argv){
    string home = getenv("HOME");
    string dir;
    //char cwd[BUFSIZ];
    //Loginv
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

    struct sigaction newAction, oldAction;
    newAction.sa_handler = sigHandler;
    sigemptyset(&newAction.sa_mask);
    newAction.sa_flags = 0;
    
    //For handeling ^C and such
    if(oldAction.sa_handler != SIG_IGN){
        if(sigaction(SIGINT, &newAction, &oldAction )< 0){
            perror("sigaction");
        }
    }
    
    //Declarations
    string input;
    vector<string> args;
    vector<string> tempArgs;
    bool comment = false;

    //While loop that repeats $ and cin command
    while(true){
        cout << user << "@" << host << "$ ";
        /*if(getcwd(cwd,BUFSIZ) == NULL){
            perror("getcwd");
        }
        dir = cwd;
        if(home == "\0"){
            perror("getenv");
        }
        if(dir.find(home,0) != string::npos){
            dir.replace(0,home.size(),"~");
        }
        cout << dir <<"$ ";*/
        
        cout.flush();//flush just to be safe
        cin.clear();
        comment = false;
        getline(cin,input);

//================Tokenize the input into multiple parts==============
        char_separator<char> delim(" ",";#>|&");
        tokenizer<char_separator<char> > mytok(input, delim);//seperat the input

        for(mytok::iterator it = mytok.begin(); it != mytok.end(); ++it){
            //cout << "token: " << *it  << endl; 
            if(*it == "exit"){
                exit(0);
            }else if(*it == "#"){
                comment = true;
            }else if(!comment){
                args.push_back(*it);
            }
        }
        concatArg(args);
        //==============RUN THE COMMANDS======================
        /*for(unsigned int j=0; j<args.size();++j){
            cout << args[j] << endl;
        }               DEBUG PURPOSES*/ 
        if(properInput(args)){
            for(unsigned int i=0 ; i<args.size();++i){
                if(args[i]==";"){
                    singleCommand(tempArgs);
                    tempArgs.clear();
                }else if(args[i]!="&&" && args[i]!="||"){
                    tempArgs.push_back(args[i]);
                }else if(args[i]=="||"){
                    if(!singleCommand(tempArgs)){
                        tempArgs.clear();
                        break; 
                    }
                    tempArgs.clear();
                }else if(args[i]=="&&"){ 
                    if(singleCommand(tempArgs)){
                        tempArgs.clear();
                        break;
                    }
                    tempArgs.clear();
                }
            }
            singleCommand(tempArgs);
            tempArgs.clear();
        }
        args.clear();
    }
    return 0;
}
