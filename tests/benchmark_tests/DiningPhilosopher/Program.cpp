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
	if(n <= 1){
		cout << "Invalid argument\n";
		exit(1);
	}
	
	cout << "MODULE main\n\n";
	for(int i=1;i<=n;i++){
		cout << "VAR f"<<i<<" : integer;\n";
		cout << "VAR t"<<i<<" : integer;\n";
		cout << "VAR e"<<i<<" : integer;\n";
	}

	cout << "\nASSIGN\n\n";
	for(int i=1;i<=n;i++){
		cout << "init(f" << i << ") := 1;\n";
		cout << "init(t" << i << ") := 1;\n";
		cout << "init(e" << i << ") := 0;\n";
	}
	cout << "\n";

	for(int i=1;i<=n;i++){
		int f2id = i+1>n?1:(i+1);
		string f1 = "f" + to_string(i);
		string f2 = "f" + to_string(f2id);
		string t = "t" + to_string(i);
		string e = "e" + to_string(i);
		cout << "next({" << t << "," << e << "," << f1 << "," << f2 <<"}):= (" << t << ">0 && " << f1 << ">0 && " << f2 << ">0" << ") [a" << i << "]: {" << t << "-1," << e << "+1," << f1 <<"-1," << f2 << "-1"<< "};\n";
		cout << "next({" << t << "," << e << "," << f1 << "," << f2 <<"}):= (" << e << ">0) [r" << i << "]: {" << t << "+1," << e << "-1," << f1 <<"+1," << f2 << "+1"<< "};\n";
	}

	cout << "\n\nSAFETYSPEC\n";
	string pr = "";
	for(int i=1;i<=n;i++){
		int beid = i-1==0?n:(i-1);
		int neid = i+1==n+1?1:(i+1);
		string eb = "e" + to_string(beid);
		string en = "e" + to_string(neid);
		string e = "e" + to_string(i);
		string cp = "((" + e + "> 0) -> (!(" + eb + ">0) && !(" + en + ">0)))";
		if(i == 1) pr += cp;
		else pr += " && " + cp;
	}
	cout << pr << endl;
}