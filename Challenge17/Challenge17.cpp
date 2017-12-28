/*
Note: Actually got to use a heuristic! Was wondering if A* or similar would come around...

Rank
Part1: 169
Part2: 186

--- Day 17: Two Steps Forward ---

You're trying to access a secure vault protected by a 4x4 grid of small rooms connected by doors. You start in the top-left room (marked S), and you can access the vault (marked V) once you reach the bottom-right room:

#########
#S| | | #
#-#-#-#-#
# | | | #
#-#-#-#-#
# | | | #
#-#-#-#-#
# | | |
####### V
Fixed walls are marked with #, and doors are marked with - or |.

The doors in your current room are either open or closed (and locked) based on the hexadecimal MD5 hash of a passcode (your puzzle input) followed by a sequence of uppercase characters representing the path you have taken so far (U for up, D for down, L for left, and R for right).

Only the first four characters of the hash are used; they represent, respectively, the doors up, down, left, and right from your current position. Any b, c, d, e, or f means that the corresponding door is open; any other character (any number or a) means that the corresponding door is closed and locked.

To access the vault, all you need to do is reach the bottom-right room; reaching this room opens the vault and all doors in the maze.

For example, suppose the passcode is hijkl. Initially, you have taken no steps, and so your path is empty: you simply find the MD5 hash of hijkl alone. The first four characters of this hash are ced9, which indicate that up is open (c), down is open (e), left is open (d), and right is closed and locked (9). Because you start in the top-left corner, there are no "up" or "left" doors to be open, so your only choice is down.

Next, having gone only one step (down, or D), you find the hash of hijklD. This produces f2bc, which indicates that you can go back up, left (but that's a wall), or right. Going right means hashing hijklDR to get 5745 - all doors closed and locked. However, going up instead is worthwhile: even though it returns you to the room you started in, your path would then be DU, opening a different set of doors.

After going DU (and then hashing hijklDU to get 528e), only the right door is open; after going DUR, all doors lock. (Fortunately, your actual passcode is not hijkl).

Passcodes actually used by Easter Bunny Vault Security do allow access to the vault if you know the right path. For example:

If your passcode were ihgpwlah, the shortest path would be DDRRRD.
With kglvqrro, the shortest path would be DDUDRLRRUDRD.
With ulqzkmiv, the shortest would be DRURDRUDDLLDLUURRDULRLDUUDDDRR.
Given your vault's passcode, what is the shortest path (the actual path, not just the length) to reach the vault?

Your puzzle answer was DURLDRRDRD.

--- Part Two ---

You're curious how robust this security solution really is, and so you decide to find longer and longer paths which still provide access to the vault. You remember that paths always end the first time they reach the bottom-right room (that is, they can never pass through it, only end in it).

For example:

If your passcode were ihgpwlah, the longest path would take 370 steps.
With kglvqrro, the longest path would be 492 steps long.
With ulqzkmiv, the longest path would be 830 steps long.
What is the length of the longest path that reaches the vault?

Your puzzle answer was 650.

Both parts of this puzzle are complete! They provide two gold stars: **

At this point, you should return to your advent calendar and try another puzzle.

Your puzzle input was njfxhljp.
*/

#include <iostream>
#include <string>
#include <queue>

#include "md5.h";

using namespace std;

struct path {
	string passCode;
	int heuristic;
	pair<int, int> loc;

	path() {
		passCode = "";
		heuristic = 0;
	}

	//Simple Manhattan distance heuristic between current point and end point
	void calcHeuristic(pair<int, int> &target) {
		heuristic = (target.first - loc.first) + (target.second - loc.second);
	}	
};

//For priority_queue compare function, create min-heap
bool operator<(const path &i, const path &j) {
	return i.heuristic > j.heuristic;
}

void main() {
	//Hardcode input and starting position
	string input = "njfxhljp";
	path currPath;
	currPath.passCode = input;
	pair<int, int> loc = { 0, 0 };
	pair<int, int> targetLoc = { 3, 3 };
	priority_queue<path> q;
	q.push(currPath);

	bool part1Found = false;
	string part1Result;
	int part2Result = 0;

	//Log tool to make sure it's still running
	int loopCount = 0;
	while (!q.empty()) {
		//Logging
		loopCount++;
		if (loopCount % 5000 == 0)
			cout << "Loops so far: " << loopCount << endl;

		currPath = q.top();
		q.pop();
		//See if we are at the goal, and set part1 or part2 as necessary
		if (currPath.loc == targetLoc) {
			if (!part1Found) {
				part1Result = currPath.passCode.substr(input.size());
				part1Found = true;
			}
			
			if (currPath.passCode.size() - input.size() > part2Result) {
				part2Result = currPath.passCode.size() - input.size();
			}
			
		}
		else {
			string hash = md5(currPath.passCode).substr(0, 4);
			for (int m = 0; m < 4; m++) {
				if (hash[m] >= 'b' && hash[m] <= 'f') {
					path newPath;
					pair<int, int> newLoc = currPath.loc;
					char dir;

					//Need to check which direction we are heading
					switch (m) {
					case 0:
						newLoc.first--;
						dir = 'U';
						break;
					case 1:
						newLoc.first++;
						dir = 'D';
						break;
					case 2:
						newLoc.second--;
						dir = 'L';
						break;
					case 3:
						newLoc.second++;
						dir = 'R';
						break;
					}
					if (newLoc.first >= 0 && newLoc.first <= 3 && newLoc.second >= 0 && newLoc.second <= 3) {
						newPath.loc = newLoc;
						newPath.calcHeuristic(targetLoc);
						newPath.passCode = currPath.passCode + dir;
						q.push(newPath);
					}
				}
			}
		}
	}

	//Result output
	cout << "Part 1 Answer (Shortest Trail): "<< part1Result << endl;
	cout << "Part 2 Answer (Longest Trail In Total Steps): " << part2Result << endl;
	
}