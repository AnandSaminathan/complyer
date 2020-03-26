#include <bits/stdc++.h>

using namespace std;

int main(int argc,char* argv[]){
	if(argc != 2){
		cout << "Incorrect number of arguments\n";
		exit(1);
	}
	int n;
	string phil_count(argv[1]);
	stringstream ss(phil_count);
	ss >> n;
	if(n < 2){
		cout << "Invalid argument\n";
		exit(1);
	}

	// LTLSPEC
	string s = "ltlspec F(";
	for(int i=1;i<=n;i++){
		string cur = "(e" + to_string(i) + ">0)";
		if(i == 1) s += cur;
		else s += " || " + cur;
	}
	s += ")";
	cout << s << endl;

	// L0
	s = "ltlspec G(";
	for(int i=1;i<=n;i++){
		string si = "!a" + to_string(i) + " && !r" + to_string(i) + "";
		if(i==1) s += si;
		else s += " && " + si; 
	}
	s += ")";
	cout << s << endl;

	// L1
	s = "ltlspec ";
	for(int i=1;i<=n;i++){
		string si = "F(a" + to_string(i) + ") && F(r" + to_string(i) + ")";
		if(i==1) s += si;
		else s += " && " + si; 
	}
	cout << s << endl;

	// L3
	s = "ltlspec !(G(";
	for(int i=1;i<=n;i++){
		string si = "F(a" + to_string(i) + ") && F(r" + to_string(i) + ")";
		if(i == 1) s += si;
		else s += " && " + si;
	}
	s += "))";
	cout << s << endl;

	//L4
	s = "ltlspec G(";
	for(int i=1;i<=n;i++){
		string si = "a" + to_string(i) + " && r" + to_string(i);
		if(i == 1) s += si;
		else s += " && " + si; 
	}
	s += ")";
	cout << s << endl;
}