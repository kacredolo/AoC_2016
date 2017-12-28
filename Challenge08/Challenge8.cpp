/*
Notes: The instructions were a little confusing. And there was an error in one of the examples
that mixed up x and y or row and column. Luckily for part 2 I was already displaying a grid so I could read
it quickly. 

Rank:
Part 1: 225
Part 2: 206

--- Day 8: Two-Factor Authentication ---

You come across a door implementing what you can only assume is an implementation of two-factor authentication after a long game of requirements telephone.

To get past the door, you first swipe a keycard (no problem; there was one on a nearby desk). Then, it displays a code on a little screen, and you type that code on a keypad. Then, presumably, the door unlocks.

Unfortunately, the screen has been smashed. After a few minutes, you've taken everything apart and figured out how it works. Now you just have to work out what the screen would have displayed.

The magnetic strip on the card you swiped encodes a series of instructions for the screen; these instructions are your puzzle input. The screen is 50 pixels wide and 6 pixels tall, all of which start off, and is capable of three somewhat peculiar operations:

rect AxB turns on all of the pixels in a rectangle at the top-left of the screen which is A wide and B tall.
rotate row y=A by B shifts all of the pixels in row A (0 is the top row) right by B pixels. Pixels that would fall off the right end appear at the left end of the row.
rotate column x=A by B shifts all of the pixels in column A (0 is the left column) down by B pixels. Pixels that would fall off the bottom appear at the top of the column.
For example, here is a simple sequence on a smaller screen:

rect 3x2 creates a small rectangle in the top-left corner:

###....
###....
.......
rotate column x=1 by 1 rotates the second column down by one pixel:

#.#....
###....
.#.....
rotate row y=0 by 4 rotates the top row right by four pixels:

....#.#
###....
.#.....
rotate column x=1 by 1 again rotates the second column down by one pixel, causing the bottom pixel to wrap back to the top:

.#..#.#
#.#....
.#.....
As you can see, this display technology is extremely powerful, and will soon dominate the tiny-code-displaying-screen market. That's what the advertisement on the back of the display tries to convince you, anyway.

There seems to be an intermediate check of the voltage used by the display: after you swipe your card, if the screen did work, how many pixels should be lit?

Your puzzle answer was 128.

--- Part Two ---

You notice that the screen is only capable of displaying capital letters; in the font it uses, each letter is 5 pixels wide and 6 tall.

After you swipe your card, what code should is the screen trying to display?

Your puzzle answer was EOARGPHYAO.

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
		int result = 0;
		vector<vector<bool>> grid(6, vector<bool>(50, 0));
		while (getline(input, line)) {
			vector<string> tokens;
			Tokenize(line, tokens, " ");

			//Rect creates a rectangle at the top left of the grid
			if (tokens[0] == "rect") {
				vector<string> dimensions;
				//Find the two dimensions separated by an x, such as 3x2 (columns x rows)
				Tokenize(tokens[1], dimensions, "x");
				for (int m = 0; m < stoi(dimensions[1]) && m < grid.size(); m++) {
					for (int n = 0; n < stoi(dimensions[0]) && n < grid[0].size(); n++) {
						grid[m][n] = true;
					}
				}
			}
			else if (tokens[0] == "rotate") {
				//Find the two numbers necessary to rotate. The row/column and the number of rotations
				vector<string> secondTokens;
				Tokenize(tokens[2], secondTokens, "=");
				int rotations = stoi(tokens[4]);

				//Do something different depending on if its a row or column
				if (tokens[1] == "row") {
					int rowNum = stoi(secondTokens[1]);
					rotations %= grid[0].size();
					std::rotate(grid[rowNum].begin(), grid[rowNum].end() - rotations, grid[rowNum].end());
				}
				else if (tokens[1] == "column") {
					int columnNum = stoi(secondTokens[1]);
					rotations %= grid.size();

					//There may be better ways to do this,
					//But transposing the column into a row in order to use std::rotate
					//Seemed simple enough for the project
					vector<bool> tempRow;
					for (int m = 0; m < grid.size(); m++) {
						tempRow.push_back(grid[m][columnNum]);
					}
					std::rotate(tempRow.begin(), tempRow.end() - rotations, tempRow.end());
					for (int m = 0; m < grid.size(); m++) {
						grid[m][columnNum] = tempRow[m];
					}
				}
			}
		}
		for (int m = 0; m < grid.size(); m++) {
			for (int n = 0; n < grid[0].size(); n++) {
				//Display the grid for part 2, remove the 0's for easy reading
				if (grid[m][n] == true)
					cout << (int)grid[m][n];
				else
					cout << " ";
				if (n % 5 == 4)
					cout << " ";

				//Count the lit up nodes for part 1
				if (grid[m][n] == true)
					result++;
			}
			cout << endl;
		}
		cout << endl<< "Number of Lit Up Nodes: "<<result << endl;
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