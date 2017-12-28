/*
Note: Only one part for this last day. This was a fun event overall, I looked forward to it each night.
It did have problems. Speed of completion is a rough way to hand out points for those that don't use fast languages.
It also promotes putting aside good coding practices and naming conventions, and ignoring edge cases or ANY sort of other input

Rank
Part 1: 87
Part 2: 75

OVERALL SCORE : 134 Points

--- Day 25: Clock Signal ---

You open the door and find yourself on the roof. The city sprawls away from you for miles and miles.

There's not much time now - it's already Christmas, but you're nowhere near the North Pole, much too far to deliver these stars to the sleigh in time.

However, maybe the huge antenna up here can offer a solution. After all, the sleigh doesn't need the stars, exactly; it needs the timing data they provide, and you happen to have a massive signal generator right here.

You connect the stars you have to your prototype computer, connect that to the antenna, and begin the transmission.

Nothing happens.

You call the service number printed on the side of the antenna and quickly explain the situation. "I'm not sure what kind of equipment you have connected over there," he says, "but you need a clock signal." You try to explain that this is a signal for a clock.

"No, no, a clock signal - timing information so the antenna computer knows how to read the data you're sending it. An endless, alternating pattern of 0, 1, 0, 1, 0, 1, 0, 1, 0, 1...." He trails off.

You ask if the antenna can handle a clock signal at the frequency you would need to use for the data from the stars. "There's no way it can! The only antenna we've installed capable of that is on top of a top-secret Easter Bunny installation, and you're definitely not-" You hang up the phone.

You've extracted the antenna's clock signal generation assembunny code (your puzzle input); it looks mostly compatible with code you worked on just recently.

This antenna code, being a signal generator, uses one extra instruction:

out x transmits x (either an integer or the value of a register) as the next value for the clock signal.
The code takes a value (via register a) that describes the signal to generate, but you're not sure how it's used. You'll have to find the input to produce the right signal through experimentation.

What is the lowest positive integer that can be used to initialize register a and cause the code to output a clock signal of 0, 1, 0, 1... repeating forever?

Your puzzle answer was 182.

--- Part Two ---

The antenna is ready. Now, all you need is the fifty stars required to generate the signal for the sleigh, but you don't have enough.

You look toward the sky in desperation... suddenly noticing that a lone star has been installed at the top of the antenna! Only 49 more to go.

If you like, you can [Retransmit the Signal].

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

bool CheckIfIsRegister(char c) {
	if (c >= 'a' && c <= 'd')
		return true;
	return false;
}
int GetValue(vector<long long> &n, string s) {
	if (CheckIfIsRegister(s[0]))
		return n[s[0] - 'a'];
	else
		return stoi(s);
}
void main() {
	ifstream input;
	input.open("Input.txt");
	if (input.is_open()) {
		string line;
		vector<string>lines;
		vector<vector<string>> tokens;
		while (getline(input, line)) {
			lines.push_back(line);
			tokens.push_back(vector<string>());
			Tokenize(line, tokens.back(), " ");

		}
		int index = 0;
		//Variable to make sure the output is alternating
		int outChecker = 0;
		//Value to put into register A each attempt
		int regStartValue = 0;
		//Only becomes true if we found a value that makes the output repeat
		bool isRepeating = false;
		vector<long long> nums(4, 0);
		while (!isRepeating) {
			//Restart basic variables
			index = 0;
			nums = { 0, 0, 0, 0 };
			nums[0] = regStartValue;
			outChecker = 0;

			//Aiming for 1000 timeschecked
			int timesChecked = 0;
			while (index < lines.size()) {
				// "cpy x y" or "cpy 5 y" copies the register or value to "y"
				if (tokens[index][0] == "cpy" && CheckIfIsRegister(tokens[index][2][0])) {
					nums[tokens[index][2][0] - 'a'] = GetValue(nums, tokens[index][1]);
				}
				// "inc x" increments register x
				else if (tokens[index][0] == "inc" && CheckIfIsRegister(tokens[index][1][0])) {
					nums[tokens[index][1][0] - 'a']++;
				}
				// "dec x" decrements register x
				else if (tokens[index][0] == "dec" && CheckIfIsRegister(tokens[index][1][0])) {
					nums[tokens[index][1][0] - 'a']--;
				}
				// "tgl" changes an instruction based on number of arguments
				else if (tokens[index][0] == "tgl") {
					//find which instruction we will be grabbing
					int jump = GetValue(nums, tokens[index][1]);

					if (index + jump >= 0 && index + jump < tokens.size()) {
						vector<string> instruction = tokens[index + jump];
						if (instruction.size() == 2) {
							if (instruction[0] == "inc")
								instruction[0] = "dec";
							else
								instruction[0] = "inc";
						}
						else if (instruction.size() == 3) {
							if (instruction[0] == "jnz")
								instruction[0] = "cpy";
							else
								instruction[0] = "jnz";
						}
						tokens[index + jump] = instruction;
					}
				}
				// "jnz x y" jumps forward or backward "y" instructions FROM "x" IF "x" is not 0
				else if (tokens[index][0] == "jnz") {
					//This is to speed up loops MASSIVELY for this SPECIFIC input
					if (index == 7) {
						nums[3] += 182 * nums[2];
						nums[2] = 0;
						nums[1] = 0;
					}

					if (GetValue(nums, tokens[index][1]) > 0) {
						index += GetValue(nums, tokens[index][2]) - 1;
					}
				}
				//Looking to find a value in register a that outputs 0,1,0,1... repeating
				else if (tokens[index][0] == "out") {
					int val = GetValue(nums, tokens[index][1]);
					//Make sure it's alternating
					if (val == outChecker) {
						outChecker = val == 0 ? 1 : 0;

						//Arbitrary number, 1000 is enough.
						timesChecked++;
						if (timesChecked == 1000) {
							isRepeating = true;
							break;
						}
					}
					//If the output breaks the repeating cycle, we can move on to the next value
					else {
						break;
					}
				}
				index++;
			}
			regStartValue++;
		}

		//Result
		regStartValue--;
		cout << regStartValue << endl;

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