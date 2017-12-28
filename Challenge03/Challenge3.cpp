#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

void main() {
	ifstream myfile;
	string line;
	string lines[3];
	int possibleTriangles = 0;
	int totalCount = 0;
	int rowCount = 0;

	myfile.open("Input.txt");
	if (myfile.is_open()) {
		// FIRST PART
		/*while (getline(myfile, line)) {
		totalCount++;
		bool isPossible = true;
		cout << line << endl;
		stringstream ss(line);
		string side;
		int sides[3];
		int sumSides[3] = { 0, 0, 0 };
		for (int m = 0; m < 3; m++) {
		ss >> ws;
		getline(ss, side, ' ');
		sides[m] = stoi(side);
		for (int n = 0; n < 3; n++) {
		if (m != n)
		sumSides[n] += sides[m];
		}
		}
		for (int m = 0; m < 3; m++) {
		if (sumSides[m] <= sides[m])
		isPossible = false;
		}
		if (isPossible)
		possibleTriangles++;
		}*/

		// SECOND PART
		while (getline(myfile, line)) {
			lines[rowCount] = line;
			rowCount++;
			if (rowCount == 3) {
				rowCount = 0;
				int sides[3][3];
				int sumSides[3][3] = { 0 };
				string side;
				for (int m = 0; m < 3; m++) {
					stringstream ss(lines[m]);
					for (int n = 0; n < 3; n++) {
						ss >> ws;
						getline(ss, side, ' ');
						sides[n][m] = stoi(side);
						for (int o = 0; o < 3; o++) {
							if (o != m) {
								sumSides[n][o] += sides[n][m];
							}
						}
					}
				}
				for (int m = 0; m < 3; m++) {
					bool isPossible = true;
					for (int n = 0; n < 3; n++) {
						if (sumSides[m][n] <= sides[m][n])
							isPossible = false;
					}
					if (isPossible)
						possibleTriangles++;
				}
			}
		}

		myfile.close();
	}
	cout << possibleTriangles << endl;
}