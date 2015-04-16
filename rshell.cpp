#include <iostream>
#include <string>

using namespace std;

int main(int argc,char **argv){
    string command;
    cout << argv[1];

    //While loop that repeats $ and cin command
    while(command != "exit"){
        cout << "$";
        cin >> command;
    }

    return 0;
}
