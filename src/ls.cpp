#include <iostream>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

using namespace std;

//Run regular ls

//Run -a
//void runPath

//Run -l
//void runFlag

//Run -R

//Run the function given
void runFunction(vector<string> parameters){
    vector<string> flags;
    vector<string> paths;
    


}
int main(int argc, char* argv[]){
    //vector declaration
    vector<string> parameters;
    vector<bool> flags(3,false);//0 is -a, 1 is -l, 2 is -R
    vector<string> paths;

    //For when no flag is passed in
    if(argc < 1){
        cout << ("Error too little arguments") << endl;
        exit(1);
    }else for(int i = 1; i < argc ; ++i){
        //Throws all the arguments into a vector string
        parameters.push_back(argv[i]);
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
                    cout << "Parameter: " << argv[i][j] << " not supported" << endl;
                    exit(1);
                }
            }
        }
    }

    /*for(unsigned int i = 0; i < parameters.size() ; ++i){
        cout << parameters[i] << endl;
    }Test input Parameter*/
    
    runFunction(parameters);
    
    return 0;
}
