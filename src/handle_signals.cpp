#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int count1 = 0;
int count2 = 0;
int count3 = 0;

void handle(int dasq) {
	count1++;
	cout << 'c' << flush;
}
void handle2(int dasq) {
	count2++;
	cout << '\\' << flush;
}
void handle3(int dasq) {
	cout << 's' << flush;
	count3++;
	if(0 != raise(SIGSTOP)) {
		perror("error with raise");
	}
	if(count3 == 3) {
		cout << "\n^c: " << count1 << "\n^\\: " << count2 << "\n^z: " << count3 << endl;
		exit(1);
	}
}

int main() {

	struct sigaction newac;
	newac.sa_handler = handle;
	sigemptyset(&newac.sa_mask);
	newac.sa_flags = 0;

	if(-1 == sigaction(SIGINT, &newac, NULL)) {
		perror("praalam sigaction");
	}

	struct sigaction newac2;
	newac2.sa_handler = handle2;
	sigemptyset(&newac2.sa_mask);
	newac2.sa_flags = 0;

	if(-1 == sigaction(SIGQUIT, &newac2, NULL)) {
		perror("praalam sigaction");
	}

	struct sigaction newac3;
	newac3.sa_handler = handle3;
	sigemptyset(&newac3.sa_mask);
	newac3.sa_flags = 0;

	if(-1 == sigaction(SIGTSTP, &newac3, NULL)) {
		perror("praalam sigaction");
	}



	while(1) {
		cout << 'x' << flush;
		sleep(1);
	}
}
