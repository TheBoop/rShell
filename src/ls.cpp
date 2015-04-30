#include <iostream>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>

using namespace std;

bool strSort(string a, string b){
    for(unsigned int i = 0; i<a.size(); ++i)
        a[i] = tolower(a[i]);
    for(unsigned int i = 0; i<b.size(); ++i)
        b[i] = tolower(b[i]);
    return a<b;
}


//Run the function given
void print_dir(char const *name, vector<bool> flags){
    DIR *dirp;
    struct dirent *dp;
    
    if((dirp = opendir(name)) == NULL) {
        perror("CANNOT OPEN PATH");
            exit(1);
        }
    
    //Have a string store all file to be sorted
    vector<string> fl;
    do{
        errno = 0;
        if((dp = readdir(dirp)) != NULL){
            if(flags[0])
                fl.push_back(dp-> d_name);
            else{
                if(dp->d_name[0] != '.')
                    fl.push_back(dp-> d_name);
            }
        }
    }while(dp != NULL);

    sort(fl.begin(),fl.end(),strSort);
    for(unsigned int i = 0; i<fl.size();++i)
        cout << fl[i] << " ";

    if(errno != 0)
        perror("Error reading directory");
        
}

int main(int argc, char* argv[]){
    //vector declaration
    vector<bool> flags(4,false);//0 is -a, 1 is -l, 2 is -R
    vector<string> paths;

    //For when no flag is passed in
    if(argc < 1){
        cout << ("Error too little arguments") << endl;
        exit(1);
    }else for(int i = 1; i < argc ; ++i){
        //Throws all the arguments into a vector string
        if(argv[i][0] == '-'){
            if(argv[i][1] == '\0'){
                cout << "No flag input after - " << endl;
                exit(1);
            }else for(int j = 1; argv[i][j] != '\0'; ++j) {
                if(argv[i][j]=='a')
                    flags[0] = true;
                else if(argv[i][j] == 'l')
                    flags[1] = true;
                else if(argv[i][j] == 'R')
                    flags[2] = true;
                else{
                    cout << "Parameter: " << argv[i][j] 
                    << " not supported" << endl;
                    exit(1);
                }
            }
        }else{ //Throws all paths into the parameter
            paths.push_back(argv[i]);
        }
    }

    /*for(unsigned int i = 0; i < parameters.size() ; ++i){
        cout << parameters[i] << endl;
    }Test input Parameter*/

    //if there is no path specified or if there is multiple paths
    if(paths.empty())
        paths.push_back(".");
    else if(paths.size() > 1)
        flags[3] = true;

    for(unsigned int i = 0; i<paths.size(); i++){
        print_dir(paths[0].c_str(),flags);
    }
    cout << "\n";
    cout.flush();
    return 0;
}
