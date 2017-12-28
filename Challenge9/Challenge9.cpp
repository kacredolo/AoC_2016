/*
Note: Distracted by Critical Role stream. Also was trying to take into account
edge cases for Part 2. The input in these problems is always very straight forward.
It could have been screwy if Part 2 had input like "(1x3)(2x2)A" which has problems on multiple fronts

Rank
Part1: 379
Part2: 432

--- Day 9: Explosives in Cyberspace ---

Wandering around a secure area, you come across a datalink port to a new part of the network. After briefly scanning it for interesting files, you find one file in particular that catches your attention. It's compressed with an experimental format, but fortunately, the documentation for the format is nearby.

The format compresses a sequence of characters. Whitespace is ignored. To indicate that some sequence should be repeated, a marker is added to the file, like (10x2). To decompress this marker, take the subsequent 10 characters and repeat them 2 times. Then, continue reading the file after the repeated data. The marker itself is not included in the decompressed output.

If parentheses or other characters appear within the data referenced by a marker, that's okay - treat it like normal data, not a marker, and then resume looking for markers after the decompressed section.

For example:

ADVENT contains no markers and decompresses to itself with no changes, resulting in a decompressed length of 6.
A(1x5)BC repeats only the B a total of 5 times, becoming ABBBBBC for a decompressed length of 7.
(3x3)XYZ becomes XYZXYZXYZ for a decompressed length of 9.
A(2x2)BCD(2x2)EFG doubles the BC and EF, becoming ABCBCDEFEFG for a decompressed length of 11.
(6x1)(1x3)A simply becomes (1x3)A - the (1x3) looks like a marker, but because it's within a data section of another marker, it is not treated any differently from the A that comes after it. It has a decompressed length of 6.
X(8x2)(3x3)ABCY becomes X(3x3)ABC(3x3)ABCY (for a decompressed length of 18), because the decompressed data from the (8x2) marker (the (3x3)ABC) is skipped and not processed further.
What is the decompressed length of the file (your puzzle input)? Don't count whitespace.

Your puzzle answer was 70186.

--- Part Two ---

Apparently, the file actually uses version two of the format.

In version two, the only difference is that markers within decompressed data are decompressed. This, the documentation explains, provides much more substantial compression capabilities, allowing many-gigabyte files to be stored in only a few kilobytes.

For example:

(3x3)XYZ still becomes XYZXYZXYZ, as the decompressed section contains no markers.
X(8x2)(3x3)ABCY becomes XABCABCABCABCABCABCY, because the decompressed data from the (8x2) marker is then further decompressed, thus triggering the (3x3) marker twice for a total of six ABC sequences.
(27x12)(20x12)(13x14)(7x10)(1x12)A decompresses into a string of A repeated 241920 times.
(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN becomes 445 characters long.
Unfortunately, the computer you brought probably doesn't have enough memory to actually decompress the file; you'll have to come up with another way to get its decompressed length.

What is the decompressed length of the file using this improved format?

Your puzzle answer was 10915059201.

Both parts of this puzzle are complete! They provide two gold stars: **
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
		string fullInput = "";
		long int length = 0;
		while (getline(input, line)) {
			fullInput += line;
		}
		string::size_type lastPos = 0;
		string::size_type pos = 0;
		
		while (string::npos != pos || string::npos != lastPos) {
			//Find the next set of brackets
			pos = fullInput.find_first_of("(", lastPos);
			//Make sure that we didn't reach the end of fullInput
			if (pos == string::npos) {
				length += fullInput.length() - lastPos;
				break;
			}
			//Add up the letters that were skipped and didn't need decompressing
			length += pos - lastPos;
			
			//Find the crucial numbers within the brackets and multiply
			vector<string> tokens;
			lastPos = fullInput.find_first_of(")", pos);
			Tokenize(fullInput.substr(pos, lastPos - pos), tokens, "(x)");
			int letters = stoi(tokens[0]);
			int repeats = stoi(tokens[1]);
			length += letters * repeats;

			//Make sure to set the next index after all the letters counted
			lastPos = lastPos + letters + 1;
		}
		cout << "RESULT 1 " << length << endl;

		//PART2
		//The edge cases (that are rarely tested in Advent of Code) tripped me up
		
		//Keep track of the sums of each index. Start from the back
		vector<long long int> sums(fullInput.length(), 0);
		for (int m = fullInput.length()-1; m >= 0; m--) {
			if (fullInput[m] == '(') {
				//Get the important numbers in the brackets, starting from the back
				pos = m;
				lastPos = fullInput.find_first_of(")", pos);
				vector<string> tokens;
				Tokenize(fullInput.substr(pos, lastPos), tokens, "(x)");
				int letters = stoi(tokens[0]);
				int repeats = stoi(tokens[1]);

				//Go through "letters" amount of indicies in front of the brackets
				//Adding all the numbers together. Set each one to 0 afterwards
				//And multiply the total sum by "repeats"
				long long int tempSum = 0;
				for (int n = lastPos + 1; n <= lastPos + letters; n++) {
					tempSum += sums[n];
					sums[n] = 0;
				}
				sums[m] = tempSum * repeats;
			}
			// A regular character is temporarily "stranded" and set to 1
			else if (fullInput[m] >= 'A' && fullInput[m] <= 'Z')
				sums[m] = 1;
		}

		//Need to go through the entire thing to make sure that no stranded characters
		//were missed. If I didn't worry about edge cases, this may not have been a necessary calculation
		long long int result2 = 0;
		for (int m = 0; m < fullInput.length(); m++) {
			result2 += sums[m];
		}
		cout << "RESULT 2 : "<<result2 << endl;

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