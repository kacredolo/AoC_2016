/*
Note: This one was deceptive. It was easy to code, but there seemed to be massive differences
between languages. What took me several minutes to run, took other people seconds with very similar code but different language.
I like the harder challenges more. These short ones are too beneficial for quick languages and bad coding practices.

Rank:
Part 1: 264
Part 2: 319

--- Day 12: Leonardo's Monorail ---

You finally reach the top floor of this building: a garden with a slanted glass ceiling. Looks like there are no more stars to be had.

While sitting on a nearby bench amidst some tiger lilies, you manage to decrypt some of the files you extracted from the servers downstairs.

According to these documents, Easter Bunny HQ isn't just this building - it's a collection of buildings in the nearby area. They're all connected by a local monorail, and there's another building not far from here! Unfortunately, being night, the monorail is currently not operating.

You remotely connect to the monorail control systems and discover that the boot sequence expects a password. The password-checking logic (your puzzle input) is easy to extract, but the code it uses is strange: it's assembunny code designed for the new computer you just assembled. You'll have to execute the code and get the password.

The assembunny code you've extracted operates on four registers (a, b, c, and d) that start at 0 and can hold any integer. However, it seems to make use of only a few instructions:

cpy x y copies x (either an integer or the value of a register) into register y.
inc x increases the value of register x by one.
dec x decreases the value of register x by one.
jnz x y jumps to an instruction y away (positive means forward; negative means backward), but only if x is not zero.
The jnz instruction moves relative to itself: an offset of -1 would continue at the previous instruction, while an offset of 2 would skip over the next instruction.

For example:

cpy 41 a
inc a
inc a
dec a
jnz a 2
dec a
The above code would set register a to 41, increase its value by 2, decrease its value by 1, and then skip the last dec a (because a is not zero, so the jnz a 2 skips it), leaving register a at 42. When you move past the last instruction, the program halts.

After executing the assembunny code in your puzzle input, what value is left in register a?

Your puzzle answer was 318083.

--- Part Two ---

As you head down the fire escape to the monorail, you notice it didn't start; register c needs to be initialized to the position of the ignition key.

If you instead initialize register c to be 1, what value is now left in register a?

Your puzzle answer was 9227737.

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
		//PART 2 SETS REGISTER "C" TO "1"
		nums[2] = 1;
		while (index < lines.size()) {
			// "cpy x y" or "cpy 5 y" copies the register or value to "y"
			if (tokens[index][0] == "cpy") {
				if (tokens[index][1][0] <= 'd' && tokens[index][1][0] >= 'a') {
					nums[tokens[index][2][0] - 'a'] = nums[tokens[index][1][0] - 'a'];
				}
				else {
					nums[tokens[index][2][0]-'a'] = stoi(tokens[index][1]);
				}
			}
			// "inc x" increments register x
			else if (tokens[index][0] == "inc") {
				nums[tokens[index][1][0] - 'a']++;
			}
			// "dec x" decrements register x
			else if (tokens[index][0] == "dec") {
				nums[tokens[index][1][0] - 'a']--;
			}
			// "jnz x y" jumps forward or backward "y" instructions FROM "x" IF "x" is not 0
			else {
				if (tokens[index][1][0] >= 'a' && tokens[index][1][0] <= 'd') {
					if (nums[tokens[index][1][0] - 'a'] > 0)
						index += stoi(tokens[index][2]) - 1;
				}
				else {
					if (stoi(tokens[index][1]) > 0)
						index += stoi(tokens[index][2]) - 1;
				}
			}
			index++;
		}
		cout << "Register A : " << nums[0] << endl;
		cout << "Register B : " << nums[0] << endl;
		cout << "Register C : " << nums[0] << endl;
		cout << "Register d : " << nums[0] << endl;
		
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