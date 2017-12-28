/*
Note: This day was weird. Part 2 the "correct" way to do it according to the maker was 
to display the input and calculate the moves by hand. Which is what most people did. 
The algorithm code wise is generally find your way to the bottom left (around walls), and then
find the way to top left (adding 5 moves per move).

Rank
Part 1: 222
Part 2: 67

--- Day 22: Grid Computing ---

You gain access to a massive storage cluster arranged in a grid; each storage node is only connected to the four nodes directly adjacent to it (three if the node is on an edge, two if it's in a corner).

You can directly access data only on node /dev/grid/node-x0-y0, but you can perform some limited actions on the other nodes:

You can get the disk usage of all nodes (via df). The result of doing this is in your puzzle input.
You can instruct a node to move (not copy) all of its data to an adjacent node (if the destination node has enough space to receive the data). The sending node is left empty after this operation.
Nodes are named by their position: the node named node-x10-y10 is adjacent to nodes node-x9-y10, node-x11-y10, node-x10-y9, and node-x10-y11.

Before you begin, you need to understand the arrangement of data on these nodes. Even though you can only move data between directly connected nodes, you're going to need to rearrange a lot of the data to get access to the data you need. Therefore, you need to work out how you might be able to shift data around.

To do this, you'd like to count the number of viable pairs of nodes. A viable pair is any two nodes (A,B), regardless of whether they are directly connected, such that:

Node A is not empty (its Used is not zero).
Nodes A and B are not the same node.
The data on node A (its Used) would fit on node B (its Avail).
How many viable pairs of nodes are there?

Your puzzle answer was 1007.

--- Part Two ---

Now that you have a better understanding of the grid, it's time to get to work.

Your goal is to gain access to the data which begins in the node with y=0 and the highest x (that is, the node in the top-right corner).

For example, suppose you have the following grid:

Filesystem            Size  Used  Avail  Use%
/dev/grid/node-x0-y0   10T    8T     2T   80%
/dev/grid/node-x0-y1   11T    6T     5T   54%
/dev/grid/node-x0-y2   32T   28T     4T   87%
/dev/grid/node-x1-y0    9T    7T     2T   77%
/dev/grid/node-x1-y1    8T    0T     8T    0%
/dev/grid/node-x1-y2   11T    7T     4T   63%
/dev/grid/node-x2-y0   10T    6T     4T   60%
/dev/grid/node-x2-y1    9T    8T     1T   88%
/dev/grid/node-x2-y2    9T    6T     3T   66%
In this example, you have a storage grid 3 nodes wide and 3 nodes tall. The node you can access directly, node-x0-y0, is almost full. The node containing the data you want to access, node-x2-y0 (because it has y=0 and the highest x value), contains 6 terabytes of data - enough to fit on your node, if only you could make enough space to move it there.

Fortunately, node-x1-y1 looks like it has enough free space to enable you to move some of this data around. In fact, it seems like all of the nodes have enough space to hold any node's data (except node-x0-y2, which is much larger, very full, and not moving any time soon). So, initially, the grid's capacities and connections look like this:

( 8T/10T) --  7T/ 9T -- [ 6T/10T]
|           |           |
6T/11T  --  0T/ 8T --   8T/ 9T
|           |           |
28T/32T  --  7T/11T --   6T/ 9T
The node you can access directly is in parentheses; the data you want starts in the node marked by square brackets.

In this example, most of the nodes are interchangable: they're full enough that no other node's data would fit, but small enough that their data could be moved around. Let's draw these nodes as .. The exceptions are the empty node, which we'll draw as _, and the very large, very full node, which we'll draw as #. Let's also draw the goal data as G. Then, it looks like this:

(.) .  G
.  _  .
#  .  .
The goal is to move the data in the top right, G, to the node in parentheses. To do this, we can issue some commands to the grid and rearrange the data:

Move data from node-y0-x1 to node-y1-x1, leaving node node-y0-x1 empty:

(.) _  G
.  .  .
#  .  .
Move the goal data from node-y0-x2 to node-y0-x1:

(.) G  _
.  .  .
#  .  .
At this point, we're quite close. However, we have no deletion command, so we have to move some more data around. So, next, we move the data from node-y1-x2 to node-y0-x2:

(.) G  .
.  .  _
#  .  .
Move the data from node-y1-x1 to node-y1-x2:

(.) G  .
.  _  .
#  .  .
Move the data from node-y1-x0 to node-y1-x1:

(.) G  .
_  .  .
#  .  .
Next, we can free up space on our node by moving the data from node-y0-x0 to node-y1-x0:

(_) G  .
.  .  .
#  .  .
Finally, we can access the goal data by moving the it from node-y0-x1 to node-y0-x0:

(G) _  .
.  .  .
#  .  .
So, after 7 steps, we've accessed the data we want. Unfortunately, each of these moves takes time, and we need to be efficient:

What is the fewest number of steps required to move your goal data to node-x0-y0?

Your puzzle answer was 242.

Both parts of this puzzle are complete! They provide two gold stars: **
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

struct node {
	int size;
	int used;
	int avail;
	int usePer;
};

void main() {
	ifstream input;
	input.open("Input.txt");


	if (input.is_open()) {
		string line;

		vector<vector<node>> grid;
		vector<vector<string>> tokens;
		while (getline(input, line)) {
			tokens.push_back(vector<string>());
			Tokenize(line, tokens.back(), " ");
		}
		//Parse input and add nodes to grid
		for (int m = 0; m < tokens.size(); m++) {
			vector<string> loc;
			Tokenize(tokens[m][0], loc, "-");
			int x = stoi(loc[1].substr(1));
			int y = stoi(loc[2].substr(1));
			
			while (grid.size() <= x)
				grid.push_back(vector<node>());
			while (grid[x].size() <= y)
				grid[x].push_back({});

			grid[x][y].size = stoi(tokens[m][1].substr(0, tokens[m][1].size() - 1));
			grid[x][y].used = stoi(tokens[m][2].substr(0, tokens[m][2].size() - 1));
			grid[x][y].avail = stoi(tokens[m][3].substr(0, tokens[m][3].size() - 1));
			grid[x][y].usePer = stoi(tokens[m][4].substr(0, tokens[m][4].size() - 1));
		}

		//Find the answer to part 1, pairs of nodes that can hold the others data (only empty node can)
		int pairsCount = 0;
		for (int m = 0; m < grid.size(); m++) {
			for (int n = 0; n < grid[m].size(); n++) {
				for (int o = 0; o < grid.size(); o++) {
					for (int p = 0; p < grid[o].size(); p++) {
						if (grid[m][n].used <= grid[o][p].avail && grid[m][n].used > 0)
							pairsCount++;
					}
				}
			}
		}
		cout << "PAIRS " << pairsCount << endl;


		//PART2
		//Did it by hand after displaying data
		pair<int, int> emptySquare;
		vector<vector<int>> grid2(grid.size(), vector<int>(grid[0].size(), 0));
		for (int m = 0; m < grid.size(); m++) {
			for (int n = 0; n < grid[m].size(); n++) {
				if (grid[m][n].used == 0) {
					grid2[m][n] = 1;
					emptySquare = { m, n };
				}
				else if (grid[m][n].used > 200)
					grid2[m][n] = 2;
				else grid2[m][n] = 0;
			}
		}
		for (int m = 0; m < grid2.size(); m++) {
			for (int n = 0; n < grid2[m].size(); n++) {
				cout << grid2[m][n] << " ";
			}
			cout << endl;
		}

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