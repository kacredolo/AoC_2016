/*
Note: I liked this one. It gave me a quick "Ah-Hah!" when I realized I could turn it into the Traveling Salesman Problem
by finding the shortest paths for each node. Also helps that I placed.

Rank
Part 1: 93
Part 2: 83

--- Day 24: Air Duct Spelunking ---

You've finally met your match; the doors that provide access to the roof are locked tight, and all of the controls and related electronics are inaccessible. You simply can't reach them.

The robot that cleans the air ducts, however, can.

It's not a very fast little robot, but you reconfigure it to be able to interface with some of the exposed wires that have been routed through the HVAC system. If you can direct it to each of those locations, you should be able to bypass the security controls.

You extract the duct layout for this area from some blueprints you acquired and create a map with the relevant locations marked (your puzzle input). 0 is your current location, from which the cleaning robot embarks; the other numbers are (in no particular order) the locations the robot needs to visit at least once each. Walls are marked as #, and open passages are marked as .. Numbers behave like open passages.

For example, suppose you have a map like the following:

###########
#0.1.....2#
#.#######.#
#4.......3#
###########
To reach all of the points of interest as quickly as possible, you would have the robot take the following path:

0 to 4 (2 steps)
4 to 1 (4 steps; it can't move diagonally)
1 to 2 (6 steps)
2 to 3 (2 steps)
Since the robot isn't very fast, you need to find it the shortest route. This path is the fewest steps (in the above example, a total of 14) required to start at 0 and then visit every other location at least once.

Given your actual map, and starting from location 0, what is the fewest number of steps required to visit every non-0 number marked on the map at least once?

Your puzzle answer was 470.

--- Part Two ---

Of course, if you leave the cleaning robot somewhere weird, someone is bound to notice.

What is the fewest number of steps required to start at 0, visit every non-0 number marked on the map at least once, and then return to 0?

Your puzzle answer was 720.

Both parts of this puzzle are complete! They provide two gold stars: **
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

int FindPathFromNodeToNode(vector<vector<int>> &grid, pair<int, int> start, pair<int, int> target);
int FindBestPath(vector<vector<int>> &d, int index, int sum, int checked, vector<bool> poss, bool isPart2);

void main() {
	ifstream input;
	input.open("Input.txt");
	if (input.is_open()) {
		string line;
		vector<vector<int>> grid;
		vector<pair<int, int>> importantNodes;

		// Create the map where 
		// '#' is a wall
		// '.' is an open space
		// '0-9' are important nodes to reach
		while (getline(input, line)) {
			grid.push_back(vector<int>());
			for (int m = 0; m < line.size(); m++) {
				if (line[m] == '#')
					grid.back().push_back(1);
				else if (line[m] == '.')
					grid.back().push_back(0);
				else {
					grid.back().push_back(2);

					//Keep the important nodes ordered. Only really important for the starting '0' node
					if (line[m] - '0' >= importantNodes.size())
						importantNodes.resize((line[m] - '0') + 1);
					importantNodes[line[m] - '0'] = { grid.size() - 1, m };
				}
			}
		}
		
		//Use BFS to collect the shortest paths from node to every other node
		vector<vector<int>> distances(importantNodes.size(), vector<int>(importantNodes.size(), 0));
		for (int m = 0; m < importantNodes.size(); m++) {
			for (int n = m + 1; n < importantNodes.size(); n++) {
				int dist = FindPathFromNodeToNode(grid, importantNodes[m], importantNodes[n]);
				distances[m][n] = dist;
				distances[n][m] = dist;
			}
		}

		vector<bool> poss(importantNodes.size(), 0);
		poss[0] = 1;

		//PART 1
		cout << "Part 1 Answer : "<<FindBestPath(distances, 0, 0, 1, poss, 0) << endl;
		//PART 2
		cout << "Part 2 Answer : "<<FindBestPath(distances, 0, 0, 1, poss, 1) << endl;

		input.close();
	}
}

//Recursive function that goes through each of the distances for each node
//Travelling Salesman Problem being brute forced, but the input is small enough to not matter
int FindBestPath(vector<vector<int>> &d, int index, int sum, int checked, vector<bool> poss, bool isPart2) {
	if (checked == poss.size()) {
		//If this is part 2, we need to also add the return trip back to the 0 node
		return isPart2 == true ? sum + d[index][0] : sum;
	}

	int bestSum = INT_MAX;
	for (int m = 0; m < d[index].size(); m++) {
		//Make sure it isn't a node we have visited before
		if (poss[m] == 0) {
			poss[m] = 1;
			int ret = FindBestPath(d, m, sum + d[index][m], checked + 1, poss, isPart2);
			poss[m] = 0;

			//Check if this taking this route is better than the others
			if (ret < bestSum)
				bestSum = ret;
		}
	}
	return bestSum;
}

//BFS Function to find shortest path from Start location to Target location
int FindPathFromNodeToNode(vector<vector<int>> &grid, pair<int, int> start, pair<int, int> target) {
	//Don't repeat locations
	set<pair<int, int>> mySet;
	mySet.insert(start);
	
	queue<pair<int, int>> q;
	queue<int> moves;
	q.push(start);
	moves.push(0);

	while (!q.empty()) {
		pair<int, int> loc = q.front();
		int cMoves = moves.front();
		q.pop();
		moves.pop();
		if (loc == target) {
			return cMoves;
		}
		//Going up
		if (loc.first - 1 >= 0 && grid[loc.first - 1][loc.second] != 1 && mySet.count({ loc.first - 1, loc.second }) == 0) {
			q.push({ loc.first - 1, loc.second });
			mySet.insert({ loc.first - 1, loc.second });
			moves.push(cMoves + 1);
		}
		//Going Down
		if (loc.first + 1 < grid.size() && grid[loc.first + 1][loc.second] != 1 && mySet.count({ loc.first + 1, loc.second }) == 0) {
			q.push({ loc.first + 1, loc.second });
			moves.push(cMoves + 1);
			mySet.insert({ loc.first + 1, loc.second });
		}
		//Going Left
		if (loc.second - 1 >= 0 && grid[loc.first][loc.second - 1] != 1 && mySet.count({ loc.first, loc.second -1}) == 0) {
			q.push({ loc.first, loc.second - 1});
			moves.push(cMoves + 1);
			mySet.insert({ loc.first, loc.second - 1 });
		}
		//Going Right
		if (loc.second + 1 < grid[loc.first].size() && grid[loc.first][loc.second+1] != 1 && mySet.count({ loc.first, loc.second + 1}) == 0) {
			q.push({ loc.first, loc.second + 1});
			moves.push(cMoves + 1);
			mySet.insert({ loc.first, loc.second + 1 });
		}
	}
}