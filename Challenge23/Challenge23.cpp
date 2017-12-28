/*
Note: This one was a bit of a mess. First I fell behind because the code from day 12 had a bug in it that didn't affect that day's input.
Then I was caught between adding a "peephole optimization" which goes against my will to code for different inputs, or let it continue running. 
Some people claimed it'd be done within 30 seconds to a minute without further optimization. Mine ran for about 20 minutes before I gave up and added the optimization

Rank
Part 1: 300
Part 2: 282

--- Day 23: Safe Cracking ---

This is one of the top floors of the nicest tower in EBHQ. The Easter Bunny's private office is here, complete with a safe hidden behind a painting, and who wouldn't hide a star in a safe behind a painting?

The safe has a digital screen and keypad for code entry. A sticky note attached to the safe has a password hint on it: "eggs". The painting is of a large rabbit coloring some eggs. You see 7.

When you go to type the code, though, nothing appears on the display; instead, the keypad comes apart in your hands, apparently having been smashed. Behind it is some kind of socket - one that matches a connector in your prototype computer! You pull apart the smashed keypad and extract the logic circuit, plug it into your computer, and plug your computer into the safe.

Now, you just need to figure out what output the keypad would have sent to the safe. You extract the assembunny code from the logic chip (your puzzle input).
The code looks like it uses almost the same architecture and instruction set that the monorail computer used! You should be able to use the same assembunny interpreter for this as you did there, but with one new instruction:

tgl x toggles the instruction x away (pointing at instructions like jnz does: positive means forward; negative means backward):

For one-argument instructions, inc becomes dec, and all other one-argument instructions become inc.
For two-argument instructions, jnz becomes cpy, and all other two-instructions become jnz.
The arguments of a toggled instruction are not affected.
If an attempt is made to toggle an instruction outside the program, nothing happens.
If toggling produces an invalid instruction (like cpy 1 2) and an attempt is later made to execute that instruction, skip it instead.
If tgl toggles itself (for example, if a is 0, tgl a would target itself and become inc a), the resulting instruction is not executed until the next time it is reached.
For example, given this program:

cpy 2 a
tgl a
tgl a
tgl a
cpy 1 a
dec a
dec a
cpy 2 a initializes register a to 2.
The first tgl a toggles an instruction a (2) away from it, which changes the third tgl a into inc a.
The second tgl a also modifies an instruction 2 away from it, which changes the cpy 1 a into jnz 1 a.
The fourth line, which is now inc a, increments a to 3.
Finally, the fifth line, which is now jnz 1 a, jumps a (3) instructions ahead, skipping the dec a instructions.
In this example, the final value in register a is 3.

The rest of the electronics seem to place the keypad entry (the number of eggs, 7) in register a, run the code, and then send the value left in register a to the safe.

What value should be sent to the safe?

Your puzzle answer was 12560.

--- Part Two ---

The safe doesn't open, but it does make several angry noises to express its frustration.

You're quite sure your logic is working correctly, so the only other thing is... you check the painting again. As it turns out, colored eggs are still eggs. Now you count 12.

As you run the program with this new input, the prototype computer begins to overheat. You wonder what's taking so long, and whether the lack of any instruction more powerful than "add one" has anything to do with it. Don't bunnies usually multiply?

Anyway, what value should actually be sent to the safe?

Your puzzle answer was 479009120.

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
		vector<long long> nums(4, 0);
		//nums[0] either equals 7 for Part 1 or 12 for Part 2
		nums[0] = 12;
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
			else if (tokens[index][0] == "jnz"){
				//This is to speed up loops MASSIVELY for this SPECIFIC input
				if (index == 9) {
					nums[0] += nums[1] * nums[3];
					nums[3] = 0;
					nums[2] = 0;
				}
				
				if (GetValue(nums, tokens[index][1]) > 0) {
					index += GetValue(nums, tokens[index][2]) - 1;
				}
			}
			index++;
		}
		cout << "Register A : " << nums[0] << endl;
		cout << "Register B : " << nums[1] << endl;
		cout << "Register C : " << nums[2] << endl;
		cout << "Register d : " << nums[3] << endl;

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