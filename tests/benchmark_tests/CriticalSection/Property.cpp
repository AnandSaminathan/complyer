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
	string s = "ltlspec ";
	for(int i=1;i<=n;i++){
		string ci = "c" + to_string(i);
		ci = ci + " > 0";
		ci = "F(" + ci + ")";
		if(i == 1) s += ci;
		else s += " && " + ci;
	}
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
