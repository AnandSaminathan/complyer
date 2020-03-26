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
	
	// SAFETYSPEC
	// cout << "safetyspec (free + used == " << n <<")\n";
	// cout << endl;

	//TODO : DETERMINE BOUND

	// LTLSPEC
	cout << "ltlspec G((pp2 > 0) -> F(cp2 > 0))\n";
	// L0
	cout << "ltlspec G(!pt1 && !pt2 && !ct1 && !ct2)\n";
	// L1
	cout << "ltlspec F(pt1) && F(pt2) && F(ct1) && F(ct2)\n";
	// L3
	cout << "ltlspec !G(F(pt1) && F(pt2) && F(ct1) && F(ct2))\n";
	// L4
	cout << "ltlspec G(pt1 && pt2 && ct1 && ct2)\n";


}
