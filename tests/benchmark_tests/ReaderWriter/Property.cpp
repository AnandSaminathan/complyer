#include <bits/stdc++.h>

using namespace std;

int main(int argc,char* argv[]){
	if(argc != 3){
		cout << "Incorrect number of arguments\n";
		exit(1);
	}
	int n,m;
	n = stoi(string(argv[1]));
	m = stoi(string(argv[2]));
	if(n < 1 || m < 1){
		cout << "Invalid argument\n";
		exit(1);
	}

	// LTLSPEC
	string s = "ltlspec ";
	for(int i=1;i<=n;i++){
		string si = "F(rc" + to_string(i) + ">0)";
		if(i==1) s += si;
		else s += " && " + si;
	}
	for(int i=1;i<=m;i++){
		string si = "F(wc" + to_string(i) + ">0)";
		s += " && " + si;
	}
	cout << s << endl;

	// L0
	s = "ltlspec G(";
	for(int i=1;i<=n;i++){
		string si = "!ra" + to_string(i) + " && !rr" + to_string(i) + "";
		if(i==1) s += si;
		else s += " && " + si; 
	}
	for(int i=1;i<=m;i++){
		string si = "!wa" + to_string(i) + " && !wr" + to_string(i) + "";
		s += " && " + si; 
	}
	s += ")";
	cout << s << endl;

	// L1
	s = "ltlspec ";
	for(int i=1;i<=n;i++){
		string si = "F(ra" + to_string(i) + ") && F(rr" + to_string(i) + ")";
		if(i==1) s += si;
		else s += " && " + si; 
	}
	for(int i=1;i<=m;i++){
		string si = "F(wa" + to_string(i) + ") && F(wr" + to_string(i) + ")";
		s += " && " + si; 
	}
	cout << s << endl;

	// L3
	s = "ltlspec !(G(";
	for(int i=1;i<=n;i++){
		string si = "F(ra" + to_string(i) + ") && F(rr" + to_string(i) + ")";
		if(i == 1) s += si;
		else s += " && " + si;
	}
	for(int i=1;i<=m;i++){
		string si = "F(wa" + to_string(i) + ") && F(wr" + to_string(i) + ")";
		s += " && " + si;
	}
	s += "))";
	cout << s << endl;

	//L4
	s = "ltlspec G(";
	for(int i=1;i<=n;i++){
		string si = "ra" + to_string(i) + " && rr" + to_string(i);
		if(i == 1) s += si;
		else s += " && " + si; 
	}
	for(int i=1;i<=m;i++){
		string si = "wa" + to_string(i) + " && wr" + to_string(i);
		s += " && " + si; 
	}
	s += ")";
	cout << s << endl;
}