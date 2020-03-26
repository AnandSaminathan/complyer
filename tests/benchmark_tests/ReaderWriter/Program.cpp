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
	
	cout << "MODULE main\n\n";
	cout << "VAR lock : integer;\n";
	for(int i=1;i<=n;i++){
		cout << "VAR rp" << i << ": integer;\n";
		cout << "VAR rc" << i << ": integer;\n";
	}
	for(int i=1;i<=m;i++){
		cout << "VAR wp" << i << ": integer;\n";
		cout << "VAR wc" << i << ": integer;\n";
	}

	cout << "\nASSIGN\n\n";
	cout << "init(lock) := " << n << ";\n";
	for(int i=1;i<=n;i++){
		cout << "init(rp" << i << ") := 1;\n";
		cout << "init(rc" << i << ") := 0;\n";
	}
	for(int i=1;i<=m;i++){
		cout << "init(wp" << i << ") := 1;\n";
		cout << "init(wc" << i << ") := 0;\n";
	}

	cout << "\n";
	for(int i=1;i<=n;i++){
		string p = "rp" + to_string(i);
		string c = "rc" + to_string(i);
		cout << "next({" << p << "," << c <<",lock}) := (" << p << " > 0 && lock > 0) [ra" << i << "]: {" << p << " - 1, "<< c << " + 1, lock - 1};\n";
		cout << "next({" << p << "," << c <<",lock}) := (" << c << " > 0) [rr" << i << "]: {" << p << " + 1, "<< c << " - 1, lock + 1};\n";
	}
	for(int i=1;i<=m;i++){
		string p = "wp" + to_string(i);
		string c = "wc" + to_string(i);
		cout << "next({" << p << "," << c <<",lock}) := (" << p << " > 0 && lock > " << (n-1) << ") [wa" << i << "]: {" << p << " - 1, "<< c << " + 1, lock - " << n << "};\n";
		cout << "next({" << p << "," << c <<",lock}) := (" << c << " > 0) [wr" << i << "]: {" << p << " + 1, "<< c << " - 1, lock + " << n << "};\n";
	}

	cout << "\nSAFETYSPEC\n";
	string prop = "";
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			string cur = "(rc" + to_string(i) + ">0) -> !(wc" + to_string(j) + ">0)";
			cur = "(" + cur + ")";
			if(i==1 && j==1) prop += cur;
			else prop += " && " + cur;
		}
	}
	for(int i=1;i<=m;i++){
		for(int j=i+1;j<=m;j++){
			string cur = "(wc" + to_string(i) + ">0) -> !(wc" + to_string(j) + ">0)";
			cur = "(" + cur + ")";
			prop += " && " + cur;
		}
	}
	cout << prop << endl;
}