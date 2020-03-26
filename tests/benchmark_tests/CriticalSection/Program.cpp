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
	cout << "VAR lock : integer;\n";
	for(int i=1;i<=n;i++){
		cout << "VAR p"<<i<<" : integer;\n";
		cout << "VAR c"<<i<<" : integer;\n";
	}

	cout << "\nASSIGN\n\n";
	cout << "init(lock) := 1;\n";
	for(int i=1;i<=n;i++){
		cout << "init(p"<<i<<") := 1;\n";
		cout << "init(c"<<i<<") := 0;\n";
	}
	cout << "\n";

	for(int i=1;i<=n;i++){
		string p = "p" + to_string(i);
		string c = "c" + to_string(i);
		cout << "next({" << p << "," << c <<",lock}) := (" << p << " > 0 && lock > 0) [a" << i << "]: {" << p << " - 1, "<< c << " + 1, lock - 1};\n";
		cout << "next({" << p << "," << c <<",lock}) := (" << c << " > 0) [r" << i << "]: {" << p << " + 1, "<< c << " - 1, lock + 1};\n";
	}

	cout << "\n\n";
	cout << "SAFETYSPEC\n";
	string prop = "";
	for(int i=1;i<=n;i++){
		for(int j=i+1;j<=n;j++){
			string cur = "(";
			cur += "(c" + to_string(i) + " > 0)";
			cur += " -> ";
			cur += "!(c" + to_string(j) + " > 0))";
			if(i==1 && j == 2){
				prop += cur;
			} else prop += " && " + cur;
		}
	}
	cout << prop << endl;
}
