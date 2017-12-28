/*
Note: Nice and easy to code, but the instructions were confusing as hell. Discs going up? Down? Spinning?
Capsule going up? Down? Are slots good things? Bouncing?

Rank
Part 1: 268
Part 2: 258

--- Day 15: Timing is Everything ---

The halls open into an interior plaza containing a large kinetic sculpture. The sculpture is in a sealed enclosure and seems to involve a set of identical spherical capsules that are carried to the top and allowed to bounce through the maze of spinning pieces.

Part of the sculpture is even interactive! When a button is pressed, a capsule is dropped and tries to fall through slots in a set of rotating discs to finally go through a little hole at the bottom and come out of the sculpture. If any of the slots aren't aligned with the capsule as it passes, the capsule bounces off the disc and soars away. You feel compelled to get one of those capsules.

The discs pause their motion each second and come in different sizes; they seem to each have a fixed number of positions at which they stop. You decide to call the position with the slot 0, and count up for each position it reaches next.

Furthermore, the discs are spaced out so that after you push the button, one second elapses before the first disc is reached, and one second elapses as the capsule passes from one disk to the one below it. So, if you push the button at time=100, then the capsule reaches the top disc at time=101, the second disc at time=102, the third disc at time=103, and so on.

The button will only drop a capsule at an integer time - no fractional seconds allowed.

For example, at time=0, suppose you see the following arrangement:

Disc #1 has 5 positions; at time=0, it is at position 4.
Disc #2 has 2 positions; at time=0, it is at position 1.
If you press the button exactly at time=0, the capsule would start to fall; it would reach the first disc at time=1. Since the first disc was at position 4 at time=0, by time=1 it has ticked one position forward. As a five-position disc, the next position is 0, and the capsule falls through the slot.

Then, at time=2, the capsule reaches the second disc. The second disc has ticked forward two positions at this point: it started at position 1, then continued to position 0, and finally ended up at position 1 again. Because there's only a slot at position 0, the capsule bounces away.

If, however, you wait until time=5 to push the button, then when the capsule reaches each disc, the first disc will have ticked forward 5+1 = 6 times (to position 0), and the second disc will have ticked forward 5+2 = 7 times (also to position 0). In this case, the capsule would fall through the discs and come out of the machine.

However, your situation has more than two discs; you've noted their positions in your puzzle input. What is the first time you can press the button to get a capsule?

Your puzzle answer was 317371.

--- Part Two ---

After getting the first capsule (it contained a star! what great fortune!), the machine detects your success and begins to rearrange itself.

When it's done, the discs are back in their original configuration as if it were time=0 again, but a new disc with 11 positions and starting at position 0 has appeared exactly one second below the previously-bottom disc.

With this new disc, and counting again starting from time=0 with the configuration in your puzzle input, what is the first time you can press the button to get another capsule?

Your puzzle answer was 2080951.

Both parts of this puzzle are complete! They provide two gold stars: **
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
		vector<pair<int, int>> discs;
		while (getline(input, line)) {
			vector<string> tokens;
			Tokenize(line, tokens, " ");
			tokens[11].pop_back();
			discs.push_back({ stoi(tokens[3]), stoi(tokens[11]) });
		}

		//PART 2
		discs.push_back({ 11, 0 });

		//Find each disc's required number in order to be slotted at [0]
		//For Example, if disc #4 has 8 locations and is at location 3 at time=0
		//Then 4 seconds will pass before we get to the disc, making it location 7 at the earliest
		//Which means we need 1, 9, 17, 25, etc in order to be slotted
		//Because 17 % 8(locations) = 1. Location 7 + 1 = 0
		for (int m = 0; m < discs.size(); m++) {
			discs[m].second = (discs[m].first - ((discs[m].second + (m+1) % discs[m].first))) ;
		}

		bool foundCapsule = false;
		bool foundPart1 = false;
		//Result variable
		int timeCount = 0;
		while (foundCapsule == false) {
			foundCapsule = true;
			for (int m = 0; m < discs.size(); m++) {
				//PART 1 OUTPUT in order to output both
				if (m == discs.size() - 1 && foundCapsule && !foundPart1) {
					cout << "Part 1 Capsule found at time : " << timeCount << endl;
					foundPart1 = true;
				}
				//(See Above Comment) Because 17 % 8(locations) = 1. Location 7 + 1 = 0
				if (timeCount % discs[m].first != discs[m].second) {
					foundCapsule = false;
					break;
				}
			}
			timeCount++;
		}

		cout << "Part 2 Capsule found at time : "<< (timeCount - 1) << endl;
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