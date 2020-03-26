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

	cout << "MODULE main\n\n";
	for(int i=1;i<=n;i++){
		cout << "VAR a" << i << " : boolean;\n";
	}

	cout << "\nASSIGN\n\n";
	for(int i=1;i<=n;i++){
		cout << "init(a" << i << ") := ";
		if(i == 1) cout << "true";
		else cout << "false";
		cout <<";\n";
	}
	cout << "\n";
	for(int i=1;i<=n;i++){
		int prev = (i == 1)?n:(i-1);
		cout << "next(a" << i << ") := a" <<prev << ";\n";
	}

	cout << "\nSAFETYSPEC\n";
	string prop = "";
	for(int i=1;i<=n;i++){
		string curprop = "(";
		for(int j=1;j<=n;j++){
			string curvar = "a" + to_string(j);
			if(i!=j) curvar = "!" + curvar;
			if(j==1) curprop += curvar;
			else curprop += " && " + curvar;
		}
		curprop += ")";
		if(i==1) prop += curprop;
		else prop += " || " + curprop;
	}
	cout << prop << endl;
}