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
                cerr<< ">: expect output file" << endl;
                return false;
            }
        }else if(argList[i] == ">>"){
            if(i+1>=argList.size()){
                cerr<< ">>: expect output file" << endl;
                return false;
            }
        }else if(argList[i] == "<"){
            if(i+1>=argList.size()){
                cerr<< "<: expect input file" << endl;
                return false;
            }
        }else if(argList[i] == "||"){
            if(i+1>=argList.size()){
                cerr<< "||: expect input file" << endl;
                return false;
            }
        }else if(argList[i] == "|"){
            if(i+1>=argList.size()){
                cerr<< "|: expect pipe output" << endl;
                return false;
            }else if(i==0){
                cerr<< "|: expect pipe input" << endl;
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
                if(isdigit(argList[i-1][0])){
                    ++i;
                    if((fd1=open(argList[i].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 
                        S_IRUSR | S_IWUSR))<0){
                        perror("open fd0");
                        exit(1);
                    }
                    int t = atoi(argList[i-2].c_str());
                    argList.erase(argList.begin()+i-2);
                    --i;
                    singleArg.pop_back();
                    if(dup2(fd1,t)<0){
                        perror("dup fd0");
                        exit(1);
                    }
                    if(close(fd1)<0){
                        perror("close fd0");
                        exit(1);
                    }

                }else{
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
                }
            }else if(argList[i] == ">>"){
                if(isdigit(argList[i-1][0])){
                    ++i;
                    if((fd1=open(argList[i].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 
                        S_IRUSR | S_IWUSR))<0){
                        perror("open fd0");
                        exit(1);
                    }
                    int t = atoi(argList[i-2].c_str());
                    argList.erase(argList.begin()+i-2);
                    --i;
                    singleArg.pop_back();
                    if(dup2(fd1,t)<0){
                        perror("dup fd0");
                        exit(1);
                    }
                    if(close(fd1)<0){
                        perror("close fd0");
                        exit(1);
                    }

                }else{
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
        //cerr << "Single Command: " << singleArg[0] << endl;
        if(execvp(arguments[0],arguments.data())<0){
            perror("execvp");
            exit(1);
        }
        exit(1);
    }else if (pid > 0) {//Parent
        //cerr << "Waiting: " << argList[0] << endl;
        if(waitpid(pid,&status,0)<0){
            perror("wait");
            return false;
        }
        //cerr<<"Finished: "<< argList[0] << endl;
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


bool pipeCommand(vector<vector<string> >commands,int mainPid,unsigned int forkNum){
    //cerr << "PipeCommand: " << forkNum << endl;
    int fd0,fd1;
    if(commands.size()==1){
        //cerr<<"======END OF RECURSION======" << endl;
        return singleCommand(commands[0]);
    }
    vector<string> argList = commands[0];
    vector<string> singleArg;
    commands.erase(commands.begin());
    //=========================PIPES================================
    int pipes[2];
    if(pipe(pipes)<0){
        perror("pipe");
        return false;
    }
    int pid = fork();
    if(pid==0){
        //cerr << "Input Redirected" << endl;
        if(close(pipes[1])<0){
            perror("close 1");
            return false;
        }
        if(dup2(pipes[0],0)<0){
            perror("dup2");
            return false;
        }
        if(close(pipes[0])<0){
            perror("close 0");
            return false;
        }
        //cerr << "Running Child pid: " << getpid() << endl;
        pipeCommand(commands,mainPid, ++forkNum);
    }else if(pid>0){
        //cerr << "Running Parent pid: " << getpid() << endl;
        if(close(pipes[0])<0){
            perror("close 0");
            return false;
        }
        if(dup2(pipes[1],1)<0){
            perror("dup2");
            return false;
        }
        if(close(pipes[1])<0){
            perror("close 0");
            return false;
        }
        //cerr << "Output Redirected" << endl;
        for(unsigned int i = 0; i<argList.size();++i){
            if(argList[i] == ">"){
                ++i;
                if((fd1=open(argList[i].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 
                    S_IRUSR | S_IWUSR))<0){
                    perror("open fd0");
                    return false;
                }
                if(dup2(fd1,1)<0){
                    perror("dup fd0");
                    return false;
                }
                if(close(fd1)<0){
                    perror("close fd0");
                    return false;
                }
            }else if(argList[i] == ">>"){
                ++i;
                if((fd1=open(argList[i].c_str(),O_WRONLY | O_CREAT | O_APPEND, 
                    S_IRUSR | S_IWUSR))<0){
                    perror("open fd0");
                    return false;
                }
                if(dup2(fd1,1)<0){
                    perror("dup fd0");
                    return false;
                }
                if(close(fd1)<0){
                    perror("close fd0");
                    return false;
                }
            }else if(argList[i] == "<"){
                ++i;
                if((fd0=open(argList[i].c_str(),O_RDONLY))<0){
                    perror("open fd1");
                    return false;
                }
                if(dup2(fd0,0)<0){
                    perror("dup fd1");
                    return false;
                }
                if(close(fd0)<0){
                    perror("close fd1");
                    return false;
                }
            }else{
                singleArg.push_back(argList[i]);
            }
        }
        
        if(getpid()==mainPid){
            //cerr << "Running Last Arg: " << argList[0]<< endl;
            int status;
            //singleCommand(argList);
            for(unsigned int k =0; k<forkNum;++k){
                if(waitpid(pid,& status,0)<0){
                    perror("wait");
                    return false;
                }
            }
        }else{
            vector<char *> arguments;
            for(size_t i = 0; i < singleArg.size(); ++i){
                arguments.push_back(&singleArg[i][0]);
            }
            //cerr << "This pid: "<<getpid() << "\nmain: " << mainPid << endl;
            arguments.push_back('\0');
            //cerr<< "Running Some Args: " << arguments[0] << endl;
            if(execvp(arguments[0],arguments.data())<0){
                perror("execvp");
                return false;
            }
        }
    }
    
    return false;
}

//============Main management for the arguments===============
bool allCommand(vector<string> argList){
    int pipeNum = 0;
    bool status=false;
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
    //Debug purpose
    /*for(unsigned int i = 0; i<commands.size();++i){
        cout << "Command "<< i << endl;
        for(unsigned int j=0; j<commands[i].size();++j){
            cout<< commands[i][j];
        }
        cout << endl;
    }*/
    if(pipeNum == 0){
        return singleCommand(argList);
        
    }else{
        status = pipeCommand(commands,getpid(),0);
    }
    return status;
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
                    allCommand(tempArgs);
                    tempArgs.clear();
                }else if(args[i]!="&&" && args[i]!="||"){
                    tempArgs.push_back(args[i]);
                }else if(args[i]=="||"){
                    if(!allCommand(tempArgs)){
                        tempArgs.clear();
                        break; 
                    }
                    tempArgs.clear();
                }else if(args[i]=="&&"){ 
                    if(allCommand(tempArgs)){
                        tempArgs.clear();
                        break;
                    }
                    tempArgs.clear();
                }
            }
            allCommand(tempArgs);
            tempArgs.clear();
        }
        args.clear();
    }
    return 0;
}
