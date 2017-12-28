/*
Note: C++, like the tortoise, will gain on the hare as the problems become more difficult

Rank:
Part 1: 126
Part 2: 206

-- - Day 7 : Internet Protocol Version 7 -- -

While snooping around the local network of EBHQ, you compile a list of IP addresses(they're IPv7, of course; IPv6 is much too limited). You'd like to figure out which IPs support TLS(transport - layer snooping).

	An IP supports TLS if it has an Autonomous Bridge Bypass Annotation, or ABBA.An ABBA is any four - character sequence which consists of a pair of two different characters followed by the reverse of that pair, such as xyyx or abba.However, the IP also must not have an ABBA within any hypernet sequences, which are contained by square brackets.

	For example :

abba[mnop]qrst supports TLS(abba outside square brackets).
abcd[bddb]xyyx does not support TLS(bddb is within square brackets, even though xyyx is outside square brackets).
aaaa[qwer]tyui does not support TLS(aaaa is invalid; the interior characters must be different).
ioxxoj[asdfgh]zxcvbn supports TLS(oxxo is outside square brackets, even though it's within a larger string).
	How many IPs in your puzzle input support TLS ?

	Your puzzle answer was 105.

	-- - Part Two-- -

	You would also like to know which IPs support SSL(super - secret listening).

	An IP supports SSL if it has an Area - Broadcast Accessor, or ABA, anywhere in the supernet sequences(outside any square bracketed sections), and a corresponding Byte Allocation Block, or BAB, anywhere in the hypernet sequences.An ABA is any three - character sequence which consists of the same character twice with a different character between them, such as xyx or aba.A corresponding BAB is the same characters but in reversed positions : yxy and bab, respectively.

	For example :

aba[bab]xyz supports SSL(aba outside square brackets with corresponding bab within square brackets).
xyx[xyx]xyx does not support SSL(xyx, but no corresponding yxy).
aaa[kek]eke supports SSL(eke in supernet with corresponding kek in hypernet; the aaa sequence is not related, because the interior character must be different).
zazbz[bzb]cdb supports SSL(zaz has no corresponding aza, but zbz has a corresponding bzb, even though zaz and zbz overlap).
How many IPs in your puzzle input support SSL ?

Your puzzle answer was 258.

Both parts of this puzzle are complete!They provide two gold stars : **
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

// Borrowed online because parsing through strings each time takes so long for a timed challenge
void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters);

void main() {
	ifstream input;
	input.open("Input.txt");
	if (input.is_open()) {
		string line;
		int result = 0;
		int result2 = 0;

		//Used Sets for Part 2 in order to check both sides
		set<string> babChecker;
		set<string> abaChecker;

		while (getline(input, line)) {
			vector<string> tokens;
			Tokenize(line, tokens, "[]");

			//PART 1 BOOLS
			bool insideBracketABBA = false;
			bool outsideBracketAbba = false;

			//PART 2 BOOLS
			bool isValidBab = false;

			for (int m = 0; m < tokens.size(); m++) {
				//PART 1
				for (int n = 3; n < tokens[m].length(); n++) {
					//Check if the four letter string fits the bill of ABBA
					if (tokens[m][n - 3] == tokens[m][n] && tokens[m][n - 2] == tokens[m][n - 1] && tokens[m][n] != tokens[m][n - 1]) {
						//There is no input where bracketed starts off first, nor is there
						//any bracketed sections side by side. They alternate
						if (m % 2 == 1) {
							insideBracketABBA = true;
							break;
						}
						else {
							outsideBracketAbba = true;
						}

					}
				}

				//PART2
				for (int n = 2; n < tokens[m].length(); n++) {
					//Check if the three letter string fits the bill of ABA
					if (tokens[m][n - 2] == tokens[m][n] && tokens[m][n - 1] != tokens[m][n]) {
						//Non-bracketed and bracketed alternate
						if (m % 2 == 1) {
							//Morph the string so ABA becomes BAB in order to check
							string temp = tokens[m].substr(n - 1, 2);
							temp += tokens[m][n-1];
							//Insert new string into set (not old string, so no morphing required on other side)
							abaChecker.insert(temp);
							//Check if OTHER set already contains this string
							if (babChecker.count(temp)) {
								isValidBab = true;
							}
						}
						else {
							//Most of the work was done in the other section
							//So just insert string and check other set
							babChecker.insert(tokens[m].substr(n - 2, 3));
							if (abaChecker.count(tokens[m].substr(n - 2, 3))) {
								isValidBab = true;
							}
						}
					}
				}

			}
			//Make sure to clear sets after each line.
			abaChecker.clear();
			babChecker.clear();

			//Incremeneters for both Part 1 and Part 2 respectively
			if (outsideBracketAbba == true && insideBracketABBA == false) {
				result++;
			}

			if (isValidBab) {
				result2++;
			}

		}
		cout << result << endl;
		cout << result2 << endl;
		input.close();
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