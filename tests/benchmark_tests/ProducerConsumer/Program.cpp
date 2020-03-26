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
	if(n < 1){
		cout << "Invalid argument\n";
		exit(1);
	}
	
	cout << "MODULE main\n\n";
	cout << "VAR pp1 : integer;\n";
	cout << "VAR pp2 : integer;\n";
	cout << "VAR cp1 : integer;\n";
	cout << "VAR cp2 : integer;\n";
	cout << "VAR free : integer;\n";
	cout << "VAR used : integer;\n";

	cout << "\nASSIGN\n\n";
	cout << "init(pp1) := 1;\n";
	cout << "init(pp2) := 0;\n";
	cout << "init(cp1) := 1;\n";
	cout << "init(cp2) := 0;\n";
	cout << "init(free) := " << n << ";\n";
	cout << "init(used) := 0;\n\n";

	cout << "next({pp1,pp2}) := (pp1 > 0) [pt1]: {pp1 - 1, pp2 + 1};\n";
	cout << "next({pp1,pp2,free,used}) := (pp2 > 0 && free > 0) [pt2]: {pp1 + 1, pp2 - 1,free - 1,used + 1};\n";
	cout << "next({cp1,cp2,free,used}) := (cp1 > 0 && used > 0) [ct1]: {cp1 - 1, cp2 + 1,free + 1,used - 1};\n";
	cout << "next({cp1,cp2}) := (cp2 > 0) [ct2]: {cp1 + 1,cp2 - 1};\n\n";

	cout << "SAFETYSPEC\n";
	cout << "(free + used) == " << n << endl;
}
