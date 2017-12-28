/*
Note : Placed in top 100 this time! 

Rank
Part 1: 113
Part 2:67

--- Day 20: Firewall Rules ---

You'd like to set up a small hidden computer here so you can use it to get back into the network later. However, the corporate firewall only allows communication with certain external IP addresses.

You've retrieved the list of blocked IPs from the firewall, but the list seems to be messy and poorly maintained, and it's not clear which IPs are allowed. Also, rather than being written in dot-decimal notation, they are written as plain 32-bit integers, which can have any value from 0 through 4294967295, inclusive.

For example, suppose only the values 0 through 9 were valid, and that you retrieved the following blacklist:

5-8
0-2
4-7
The blacklist specifies ranges of IPs (inclusive of both the start and end value) that are not allowed. Then, the only IPs that this firewall allows are 3 and 9, since those are the only numbers not in any range.

Given the list of blocked IPs you retrieved from the firewall (your puzzle input), what is the lowest-valued IP that is not blocked?

Your puzzle answer was 32259706.

--- Part Two ---

How many IPs are allowed by the blacklist?

Your puzzle answer was 113.

Both parts of this puzzle are complete! They provide two gold stars: **
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
		
		//Grab the ranges from the input
		vector<pair<unsigned long long, unsigned long long>> nums;
		while (getline(input, line)) {
			vector<string> tokens;
			Tokenize(line, tokens, "-");
			nums.push_back({stoll(tokens[0]), stoll(tokens[1])});
		}

		//Sort all the ranges based on the low part of the range
		std::sort(nums.begin(), nums.end());

		bool foundPart1 = false;
		unsigned long long part1Result = 0;
		unsigned long long part2Result = 0;
		
		unsigned long long counter = 0;

		for (int m = 0; m < nums.size(); m++) {
			//Check if there is a gap in the ranges allowing available IP's
			if (nums[m].first > counter) {
				//Only need the first gap for part 1
				if (!foundPart1) {
					part1Result = counter;
					foundPart1 = true;
				}
				
				//Add the number of available IP's to part 2 result
				part2Result += (nums[m].first) - counter;

				//Go to the next possible available IP
				counter = nums[m].second + 1;
			}
			//Make sure the current range isn't within the previous range
			//This is to check if there are ranges such as (0, 10) and (2, 4)
			else if (nums[m].second > counter) {
				//Go to the next possible available IP
				counter = nums[m].second + 1;
				if (!foundPart1)
					part1Result = counter;
			}
		}
		cout << "Part 1 Result : "<<part1Result << endl;
		cout << "Part 2 Result : "<<part2Result << endl;
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