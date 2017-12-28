/* --- Day 1: No Time for a Taxicab ---

Santa's sleigh uses a very high-precision clock to guide its movements, and the clock's oscillator is regulated by stars. Unfortunately, the stars have been stolen... by the Easter Bunny. To save Christmas, Santa needs you to retrieve all fifty stars by December 25th.

Collect stars by solving puzzles. Two puzzles will be made available on each day in the advent calendar; the second puzzle is unlocked when you complete the first. Each puzzle grants one star. Good luck!

You're airdropped near Easter Bunny Headquarters in a city somewhere. "Near", unfortunately, is as close as you can get - the instructions on the Easter Bunny Recruiting Document the Elves intercepted start here, and nobody had time to work them out further.

The Document indicates that you should start at the given coordinates (where you just landed) and face North. Then, follow the provided sequence: either turn left (L) or right (R) 90 degrees, then walk forward the given number of blocks, ending at a new intersection.

There's no time to follow such ridiculous instructions on foot, though, so you take a moment and work out the destination. Given that you can only walk on the street grid of the city, how far is the shortest path to the destination?

For example:

Following R2, L3 leaves you 2 blocks East and 3 blocks North, or 5 blocks away.
R2, R2, R2 leaves you 2 blocks due South of your starting position, which is 2 blocks away.
R5, L5, R5, R3 leaves you 12 blocks away.
How many blocks away is Easter Bunny HQ?

Your puzzle answer was 161.

--- Part Two ---

Then, you notice the instructions continue on the back of the Recruiting Document. Easter Bunny HQ is actually at the first location you visit twice.

For example, if your instructions are R8, R4, R4, R8, the first location you visit twice is 4 blocks away, due East.

How many blocks away is the first location you visit twice?

Your puzzle answer was 110.

Both parts of this puzzle are complete! They provide two gold stars: */


#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

void main() {
	string input = "R4, R1, L2, R1, L1, L1, R1, L5, R1, R5, L2, R3, L3, L4, R4, R4, R3, L5, L1, R5, R3, L4, R1, R5, L1, R3, L2, R3, R1, L4, L1, R1, L1, L5, R1, L2, R2, L3, L5, R1, R5, L1, R188, L3, R2, R52, R5, L3, R79, L1, R5, R186, R2, R1, L3, L5, L2, R2, R4, R5, R5, L5, L4, R5, R3, L4, R4, L4, L4, R5, L4, L3, L1, L4, R1, R2, L5, R3, L4, R3, L3, L5, R1, R1, L3, R2, R1, R2, R2, L4, R5, R1, R3, R2, L2, L2, L1, R2, L1, L3, R5, R1, R4, R5, R2, R2, R4, R4, R1, L3, R4, L2, R2, R1, R3, L5, R5, R2, R5, L1, R2, R4, L1, R5, L3, L3, R1, L4, R2, L2, R1, L1, R4, R3, L2, L3, R3, L2, R1, L4, R5, L1, R5, L2, L1, L5, L2, L5, L2, L4, L2, R3";
	stringstream ss(input);
	string currentMove;

	char currentDir;
	int currentMoves;
	int totalVert = 0;
	int totalHoriz = 0;
	int totalMoves = 0;

	// Alternate which number to affect, horizontal then vertical
	bool horizontal = true;

	// Depending on direction facing, Right and Left may be Positive and Negative respectively, or reversed
	// Going Right affects the direction you are NOT currently affecting (horizontal makes vertical changed)
	// Going Left affects the direction you are CURRENTLY affecting (horizontal makes horizontal changed)
	int horizMult = 1;
	int vertMult = 1;

	// Part 2 variables
	int prevVisit = -1;
	std::unordered_map<string, bool> map;

	while (getline(ss, currentMove, ',')) {
		//Parse the input
		ss >> ws;
		currentDir = currentMove[0];
		currentMoves = stoi(currentMove.substr(1));

		int *dirMult;
		int *dirTotal;

		//Which direction we are currently changing is important
		//Could have lessened the code by using references to the appropriate variables
		if (horizontal) {
			int mult = currentDir == 'R' ? horizMult : -horizMult;
			// Part 2 made it necessary to go one step at a time
			for (int m = 0; m < currentMoves; m++) {
				totalHoriz += 1 * mult;
				if (prevVisit == -1) {
					string prevLoc = to_string(totalHoriz) + "," + to_string(totalVert);

					if (map.count(prevLoc) == 1)
						prevVisit = abs(totalVert) + abs(totalHoriz);
					else
						map.emplace(prevLoc, 1);
				}
			}
			//Explanation by variables horizMult/vertMult
			if (currentDir == 'R')
				vertMult = -vertMult;
			else
				horizMult = -horizMult;
		}
		else {
			int mult = currentDir == 'R' ? vertMult : -vertMult;
			for (int m = 0; m < currentMoves; m++) {
				totalVert += 1 * mult;
				if (prevVisit == -1) {
					string prevLoc = to_string(totalHoriz) + "," + to_string(totalVert);

					if (map.count(prevLoc) == 1)
						prevVisit = abs(totalVert) + abs(totalHoriz);
					else
						map.emplace(prevLoc, 1);
				}
			}
			if (currentDir == 'R')
				horizMult = -horizMult;
			else
				vertMult = -vertMult;


		}
		horizontal = horizontal == true ? false : true;

	}
	totalMoves = abs(totalVert) + abs(totalHoriz);
	cout << "PREV PLACE : " << prevVisit << endl;
	cout << totalMoves << endl;
}