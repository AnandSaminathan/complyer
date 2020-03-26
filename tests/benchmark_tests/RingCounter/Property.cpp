#include <bits/stdc++.h>

using namespace std;

int main(int argc,char* argv[]){
	if(argc != 2){
		cout << "Incorrect number of arguments\n";
		exit(1);
	}
	int n = stoi(string(argv[1]));
	if(n < 2) {
		cout << "Invalid argument\n";
		exit(1);
	}

	string s = "ltlspec G(";
	for(int i=1;i<=n;i++){
		string cur = "F(a" + to_string(i) + ")";
		if(i==1) s += cur;
		else s += " && " + cur; 
	}
	s += ")";
	cout << s << endl;
}