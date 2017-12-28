/*
Note: Smooth sailing. Was distracted by streams a bit. Started late.

Rank
Part 1: 188
Part 2: 148

--- Day 13: A Maze of Twisty Little Cubicles ---

You arrive at the first floor of this new building to discover a much less welcoming environment than the shiny atrium of the last one. Instead, you are in a maze of twisty little cubicles, all alike.

Every location in this area is addressed by a pair of non-negative integers (x,y). Each such coordinate is either a wall or an open space. You can't move diagonally. The cube maze starts at 0,0 and seems to extend infinitely toward positive x and y; negative values are invalid, as they represent a location outside the building. You are in a small waiting area at 1,1.

While it seems chaotic, a nearby morale-boosting poster explains, the layout is actually quite logical. You can determine whether a given x,y coordinate will be a wall or an open space using a simple system:

Find x*x + 3*x + 2*x*y + y + y*y.
Add the office designer's favorite number (your puzzle input).
Find the binary representation of that sum; count the number of bits that are 1.
If the number of bits that are 1 is even, it's an open space.
If the number of bits that are 1 is odd, it's a wall.
For example, if the office designer's favorite number were 10, drawing walls as # and open spaces as ., the corner of the building containing 0,0 would look like this:

0123456789
0 .#.####.##
1 ..#..#...#
2 #....##...
3 ###.#.###.
4 .##..#..#.
5 ..##....#.
6 #...##.###
Now, suppose you wanted to reach 7,4. The shortest route you could take is marked as O:

0123456789
0 .#.####.##
1 .O#..#...#
2 #OOO.##...
3 ###O#.###.
4 .##OO#OO#.
5 ..##OOO.#.
6 #...##.###
Thus, reaching 7,4 would take a minimum of 11 steps (starting from your current location, 1,1).

What is the fewest number of steps required for you to reach 31,39?

Your puzzle answer was 92.

--- Part Two ---

How many locations (distinct x,y coordinates, including your starting location) can you reach in at most 50 steps?

Your puzzle answer was 124.

Both parts of this puzzle are complete! They provide two gold stars: **


Your puzzle input was 1350.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

int calcBits(long long n);

void main() {
	int fav = 1350;
	pair<int, int> loc = { 1, 1 };
	pair<int, int> targetLoc = {31, 39 };
	queue<pair<int, int>> q;
	queue<int> moves;
	set<pair<int, int>> s;
	q.push(loc);
	moves.push(0);
	s.insert(loc);

	//PART 2
	int locsMovedTo = 0;

	bool part1 = false;
	bool part2 = false;

	while (!q.empty()) {
		loc = q.front();
		locsMovedTo++;
		int currMoves = moves.front();
		q.pop();
		moves.pop();

		//Check for answers to part 1 and part 2, exit out of loop if found both
		if (loc == targetLoc && !part1) {
			part1 = true;
			cout << "Reached ("<<targetLoc.first<<","<<targetLoc.second<<") in " << currMoves << " moves."<<endl;
		}
		if (currMoves > 50 && !part2) {
			part2 = true;
			locsMovedTo--;
			cout << "Locations within 50 moves: "<< locsMovedTo << endl;
		}
		if (part1 && part2)
			break;


		for (int m = -1; m <= 1; m++) {
			for (int n = -1; n <= 1; n++) {
				//Can't move diagonally, so either m or n needs to be 0
				if ((m != 0 && n == 0) || (m == 0 && n != 0)) {
					int newX = loc.first + m;
					int newY = loc.second + n;
					//Check if new loc is within map bounds and that it hasn't been visited
					if (newX >= 0 && newY >= 0 && s.count({ newX, newY }) == 0) {
						s.insert({ newX, newY });

						//Use problem arithmetic (x*x) + (3*x) + (2*x*y) + y + (y*y) + "Problem Input"
						long long temp = (newX * newX) + (3 * newX) + (2 * newX * newY) + newY + (newY * newY);
						temp += fav;

						//Calculate number of turned one bits. Even Number is open space, Odd is wall
						int bits = calcBits(temp);
						if (bits % 2 == 0) {
							q.push({ newX, newY });
							moves.push(currMoves + 1);
						}
					}
				}
			}
		}
	}
}

int calcBits(long long n) {
	int bits = 0;
	while (n > 0) {
		if (n & 1)
			bits++;
		n = n >> 1;
	}
	return bits;
}