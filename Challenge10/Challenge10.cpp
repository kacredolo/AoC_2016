/*
Rank:
Part 1: 122
Part 2: 150

--- Day 10: Balance Bots ---

You come upon a factory in which many robots are zooming around handing small microchips to each other.

Upon closer examination, you notice that each bot only proceeds when it has two microchips, and once it does, it gives each one to a different bot or puts it in a marked "output" bin. Sometimes, bots take microchips from "input" bins, too.

Inspecting one of the microchips, it seems like they each contain a single number; the bots must use some logic to decide what to do with each chip. You access the local control computer and download the bots' instructions (your puzzle input).

Some of the instructions specify that a specific-valued microchip should be given to a specific bot; the rest of the instructions indicate what a given bot should do with its lower-value or higher-value chip.

For example, consider the following instructions:

value 5 goes to bot 2
bot 2 gives low to bot 1 and high to bot 0
value 3 goes to bot 1
bot 1 gives low to output 1 and high to bot 0
bot 0 gives low to output 2 and high to output 0
value 2 goes to bot 2
Initially, bot 1 starts with a value-3 chip, and bot 2 starts with a value-2 chip and a value-5 chip.
Because bot 2 has two microchips, it gives its lower one (2) to bot 1 and its higher one (5) to bot 0.
Then, bot 1 has two microchips; it puts the value-2 chip in output 1 and gives the value-3 chip to bot 0.
Finally, bot 0 has two microchips; it puts the 3 in output 2 and the 5 in output 0.
In the end, output bin 0 contains a value-5 microchip, output bin 1 contains a value-2 microchip, and output bin 2 contains a value-3 microchip. In this configuration, bot number 2 is responsible for comparing value-5 microchips with value-2 microchips.

Based on your instructions, what is the number of the bot that is responsible for comparing value-61 microchips with value-17 microchips?

Your puzzle answer was 116.

--- Part Two ---

What do you get if you multiply together the values of one chip in each of outputs 0, 1, and 2?

Your puzzle answer was 23903.

Both parts of this puzzle are complete! They provide two gold stars: **
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

// Borrowed online because parsing through strings each time takes so long for a timed challenge
void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters);

//First challenge in Advent of Code 2016 to really feel like struct/class could be helpful
struct bot {
	int low;
	int high;
	int lowTarget;
	bool lowTargetOutput;
	int highTarget;
	bool highTargetOutput;

	bot() {
		low = -1;
		high = -1;
		lowTarget = -1;
		highTarget = -1;
		lowTargetOutput = false;
		highTargetOutput = false;
	}
	//Makes sure that low and high are in proper order
	//And returns true if both spots are filled
	bool addChip(int x) {
		if (low == -1 && high == -1)
			low = x;
		else {
			int temp = max(low, high);
			low = min(x, temp);
			high = max(x, temp);
			return true;
		}
		return false;
	}
	//Specifically for part 1, finding the bot that contains two specific numbers
	bool part1Checker(int x, int y) {
		if (low == x && high == y)
			return true;
		return false;
	}
	void removeChips() {
		low = -1;
		high = -1;
	}
};

void main() {
	ifstream input;
	input.open("Input.txt");
	if (input.is_open()) {
		string line;
		vector<bot> bots(1000);
		vector<int> outputs(1000, 0);
		queue<int> fullBots;
		while (getline(input, line)) {
			vector<string> tokens;
			Tokenize(line, tokens, " ");

			//Parse the input and fill in low, high, lowtarget, hightarget, etc
			if (tokens[0] == "value") {
				if (bots[stoi(tokens[5])].addChip(stoi(tokens[1]))) {
					//If the bot is filled with 2 chips, add it to the queue of fullbots
					fullBots.push(stoi(tokens[5]));
				}
			}
			else {
				int fromBot = stoi(tokens[1]);
				bots[fromBot].lowTarget = stoi(tokens[6]);
				bots[fromBot].highTarget = stoi(tokens[11]);
				if (tokens[5] == "output") {
					bots[fromBot].lowTargetOutput = true;
				}
				if (tokens[10] == "output") {
					bots[fromBot].highTargetOutput = true;
				}
			}
		}
		int result = 0;
		while (!fullBots.empty()) {
			int currentBot = fullBots.front();
			//Part 1 checker, looking for bot that has two specific numbers
			if (bots[currentBot].part1Checker(17, 61)) {
				result = currentBot;
			}
			fullBots.pop();

			//Move the values from bot to bot, or from bot to output
			if (!bots[currentBot].highTargetOutput && bots[bots[currentBot].highTarget].addChip(bots[currentBot].high))
				fullBots.push(bots[currentBot].highTarget);
			if (!bots[currentBot].lowTargetOutput && bots[bots[currentBot].lowTarget].addChip(bots[currentBot].low))
				fullBots.push(bots[currentBot].lowTarget);

			if (bots[currentBot].lowTargetOutput == true) {
				outputs[bots[currentBot].lowTarget] = bots[currentBot].low;
			}
			if (bots[currentBot].highTargetOutput == true) {
				outputs[bots[currentBot].highTarget] = bots[currentBot].high;
			}

			//Make sure to remove the chips from the currentbot
			bots[currentBot].removeChips();
		}
		//Part 1 Result
		cout << result << endl;
		//Part 2 asks for the multiplication of the numbers in output 0, 1 and 2
		cout << outputs[0] * outputs[1] *outputs[2] << endl;
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