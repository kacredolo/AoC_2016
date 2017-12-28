/*
RANK:
Part 1: 255
Part 2: 256

--- Day 4: Security Through Obscurity ---

Finally, you come across an information kiosk with a list of rooms. Of course, the list is encrypted and full of decoy data, but the instructions to decode the list are barely hidden nearby. Better remove the decoy data first.

Each room consists of an encrypted name (lowercase letters separated by dashes) followed by a dash, a sector ID, and a checksum in square brackets.

A room is real (not a decoy) if the checksum is the five most common letters in the encrypted name, in order, with ties broken by alphabetization. For example:

aaaaa-bbb-z-y-x-123[abxyz] is a real room because the most common letters are a (5), b (3), and then a tie between x, y, and z, which are listed alphabetically.
a-b-c-d-e-f-g-h-987[abcde] is a real room because although the letters are all tied (1 of each), the first five are listed alphabetically.
not-a-real-room-404[oarel] is a real room.
totally-real-room-200[decoy] is not.
Of the real rooms from the list above, the sum of their sector IDs is 1514.

What is the sum of the sector IDs of the real rooms?

Your puzzle answer was 173787.

--- Part Two ---

With all the decoy data out of the way, it's time to decrypt this list and get moving.

The room names are encrypted by a state-of-the-art shift cipher, which is nearly unbreakable without the right software. However, the information kiosk designers at Easter Bunny HQ were not expecting to deal with a master cryptographer like yourself.

To decrypt a room name, rotate each letter forward through the alphabet a number of times equal to the room's sector ID. A becomes B, B becomes C, Z becomes A, and so on. Dashes become spaces.

For example, the real name for qzmt-zixmtkozy-ivhz-343 is very encrypted name.

What is the sector ID of the room where North Pole objects are stored?

Your puzzle answer was 548.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

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
		int totalSum = 0;
		while (getline(input, line)) {

			//PARSING STRING, SEPARATING TOKENS, SECTORID, AND CHECKSUM TO VARIABLES
			vector<string> token;
			vector<int> counts(26, 0);
			vector<int> countsSorted(26, 0);
			Tokenize(line, token, "[");
			string checksum = token.back().erase(token.back().size()-1);
			line = token[0];
			token.clear();
			Tokenize(line, token, "-");
			int sectorID = stoi(token.back());
			token.pop_back();

			//Count number of times letters show up
			for (int m = 0; m < token.size(); m++) {
				for (int n = 0; n < token[m].size(); n++) {
					counts[token[m][n] - 'a']++;
				}
			}

			//Have a separate sorted version so I can reference letters to the highest counts
			countsSorted = counts;
			std::sort(countsSorted.begin(), countsSorted.end());
			bool realRoom = true;
			for (int m = 0; m < checksum.size(); m++) {
				if (counts[checksum[m] - 'a'] == countsSorted.back()) {
					//Make sure that if another letter has the same count
					//That it is in alphabetical order
					for (int n = m + 1; n < checksum.size(); n++) {
						if (counts[checksum[n] - 'a'] == counts[checksum[m] - 'a'] && checksum[m] > checksum[n]) {
							realRoom = false;
							break;
						}
					}
					//Remove the highest count
					countsSorted.pop_back();
				}
				else {
					realRoom = false;
				}
			}
			if (realRoom)
				totalSum += sectorID;

			//PART 2
			//Find out how many letters to traverse (A -> B		 Z -> A )
			int sipher = sectorID % 26;
			for (int m = 0; m < token.size(); m++) {
				for (int n = 0; n < token[m].size(); n++) {
					token[m][n] = token[m][n] + sipher;
					//Make sure they stay within range. For some reason ASCII is funky ('z' + 1) is not greater than ('z')?
					if (token[m][n] > 'z' || token[m][n] < 'a')
						token[m][n] -= 26;
				}
			}
			//Check for the answer to part 2, wasn't sure exactly what it wanted
			if (token[0] == "northpole" || (token[0] == "north" && token[1] == "pole"))
				cout << sectorID << endl;
		}

		cout << totalSum << endl;
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