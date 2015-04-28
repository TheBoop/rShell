#include <iostream>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

using namespace std;

//Run the function given

int main(int argc, char* argv[]){
    //vector declaration
    vector<string> parameters;
    vector<string> flags;
    vector<string> paths;
    
    //For when no flag is passed in
    if(argc < 1){
    
    }else for(int i = 1; i < argc ; i++){
        //Throws all the arguments into a vector string
        parameters.push_back(argv[i]);
    }

    /*for(unsigned int i = 0; i < parameters.size() ; ++i){
        cout << parameters[i] << endl;
    }Test input Parameter*/

}
