/*
Note: This was comprised mostly of just filling in what should happen for each rule.
Unfortunately part 1 and part 2 are conflicted with one another so the code as it stands only solves part 2

Rank
Part 1: 208
Part 2: 190

--- Day 21: Scrambled Letters and Hash ---

The computer system you're breaking into uses a weird scrambling function to store its passwords. It shouldn't be much trouble to create your own scrambled password so you can add it to the system; you just have to implement the scrambler.

The scrambling function is a series of operations (the exact list is provided in your puzzle input). Starting with the password to be scrambled, apply each operation in succession to the string. The individual operations behave as follows:

swap position X with position Y means that the letters at indexes X and Y (counting from 0) should be swapped.
swap letter X with letter Y means that the letters X and Y should be swapped (regardless of where they appear in the string).
rotate left/right X steps means that the whole string should be rotated; for example, one right rotation would turn abcd into dabc.
rotate based on position of letter X means that the whole string should be rotated to the right based on the index of letter X (counting from 0) as determined before this instruction does any rotations. Once the index is determined, rotate the string to the right one time, plus a number of times equal to that index, plus one additional time if the index was at least 4.
reverse positions X through Y means that the span of letters at indexes X through Y (including the letters at X and Y) should be reversed in order.
move position X to position Y means that the letter which is at index X should be removed from the string, then inserted such that it ends up at index Y.
For example, suppose you start with abcde and perform the following operations:

swap position 4 with position 0 swaps the first and last letters, producing the input for the next step, ebcda.
swap letter d with letter b swaps the positions of d and b: edcba.
reverse positions 0 through 4 causes the entire string to be reversed, producing abcde.
rotate left 1 step shifts all letters left one position, causing the first letter to wrap to the end of the string: bcdea.
move position 1 to position 4 removes the letter at position 1 (c), then inserts it at position 4 (the end of the string): bdeac.
move position 3 to position 0 removes the letter at position 3 (a), then inserts it at position 0 (the front of the string): abdec.
rotate based on position of letter b finds the index of letter b (1), then rotates the string right once plus a number of times equal to that index (2): ecabd.
rotate based on position of letter d finds the index of letter d (4), then rotates the string right once, plus a number of times equal to that index, plus an additional time because the index was at least 4, for a total of 6 right rotations: decab.
After these steps, the resulting scrambled password is decab.

Now, you just need to generate a new scrambled password and you can access the system. Given the list of scrambling operations in your puzzle input, what is the result of scrambling abcdefgh?

Your puzzle answer was hcdefbag.

The first half of this puzzle is complete! It provides one gold star: *

--- Part Two ---

You scrambled the password correctly, but you discover that you can't actually modify the password file on the system. You'll need to un-scramble one of the existing passwords by reversing the scrambling process.

What is the un-scrambled version of the scrambled password fbgdceah?
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// Borrowed online because parsing through strings each time takes so long for a timed challenge
void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters);

void SetMap(map<char, int> &myMap, string &s);

void main() {
	ifstream input;
	input.open("Input.txt");

	string s = "fbgdceah";

	//This map holds the index of each char, for faster lookup. 
	map<char, int> charIndex;
	SetMap(charIndex, s);

	if (input.is_open()) {
		string line;

		//Get and store ALL the input, so part 2 can go through it reversed
		vector<vector<string>> tokens;
		while (getline(input, line)) {
			tokens.push_back(vector<string>());
			Tokenize(line, tokens.back(), " ");
		}

		for (int m = tokens.size()-1; m >= 0; m--) {
			if (tokens[m][0] == "swap" && tokens[m][1] == "position") {
				//Simple swap, works both for part 1 and part 2
				int index1 = stoi(tokens[m][2]);
				int index2 = stoi(tokens[m][5]);

				char temp = s[index1];
				s[index1] = s[index2];
				s[index2] = temp;
			}
			else if (tokens[m][0] == "swap" && tokens[m][1] == "letter") {
				//Simple swap, works both for part 1 and part 2
				char c1 = tokens[m][2][0];
				char c2 = tokens[m][5][0];

				int index1 = charIndex[c1];
				int index2 = charIndex[c2];

				char temp = s[index1];
				s[index1] = s[index2];
				s[index2] = temp;
			}
			else if (tokens[m][0] == "rotate" && tokens[m][1] == "based") {
				//PART1
				/*int n = charIndex[tokens[m][6][0]];
				rotate(s.begin(), s.end() - 1, s.end());
				rotate(s.begin(), s.end() - n, s.end());
				if (n >= 4)
					rotate(s.begin(), s.end() - 1, s.end());*/

				//Part2
				int currIndex = charIndex[tokens[m][6][0]];
				int targetIndex;
				//The specifics of this rule are tricky. We need to find which
				//index would end up rotated to where it is now, in order to reverse
				for (int n = 0; n < s.size(); n++) {
					if (currIndex == (n + 1 + n + (n >= 4 ? 1 : 0)) % s.size()) {
						targetIndex = n;
						break;
					}
				}
				//Rotate left or right depending on the current index and target index
				if (currIndex < targetIndex) {
					rotate(s.begin(), s.end() - (targetIndex - currIndex), s.end());
				}
				else {
					rotate(s.begin(), s.begin() + (currIndex - targetIndex), s.end());
				}

			}
			else if (tokens[m][0] == "rotate") {
				//Simple rotate, how to set middle is reversed between part1 and part2
				string::iterator middle;
				if (tokens[m][1] == "left")
					middle = s.end() - stoi(tokens[m][2]);
				else
					middle = s.begin() + stoi(tokens[m][2]);
					
				rotate(s.begin(), middle, s.end());
			}
			else if (tokens[m][0] == "reverse") {
				//Simple reverse, works for both part 1 and part 2
				int c1 = stoi(tokens[m][2]);
				int c2 = stoi(tokens[m][4]);

				int low = min(c1, c2);
				int high = max(c1, c2);
				reverse(s.begin() + low, s.begin() + high + 1);
			}
			else if (tokens[m][0] == "move") {
				//std::String erase and insert work fine for this. Works for both parts
				int index2 = stoi(tokens[m][2]);
				int index1 = stoi(tokens[m][5]);
				string temp(1, s[index1]);
				s.erase(index1, 1);
				s.insert(index2, temp);
			}
			//Resync the indexes
			SetMap(charIndex, s);
			
		}
		cout << s << endl;
		input.close();
	}
}

void SetMap(map<char, int> &myMap, string &s) {
	for (int m = 0; m < s.size(); m++) {
		myMap[s[m]] = m;
	}
}

void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters = " ")
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}