/*
Note: I got held up by my own small mistake when finding the min/max. 
Simple mistake cost me at least 10 minutes because I'm a doofus

Rank:
Part 1: 542
Part 2: 544

--- Day 6: Signals and Noise ---

Something is jamming your communications with Santa. Fortunately, your signal is only partially jammed, and protocol in situations like this is to switch to a simple repetition code to get the message through.

In this model, the same message is sent repeatedly. You've recorded the repeating message signal (your puzzle input), but the data seems quite corrupted - almost too badly to recover. Almost.

All you need to do is figure out which character is most frequent for each position. For example, suppose you had recorded the following messages:

eedadn
drvtee
eandsr
raavrd
atevrs
tsrnev
sdttsa
rasrtv
nssdts
ntnada
svetve
tesnvt
vntsnd
vrdear
dvrsen
enarar
The most common character in the first column is e; in the second, a; in the third, s, and so on. Combining these characters returns the error-corrected message, easter.

Given the recording in your puzzle input, what is the error-corrected version of the message being sent?

Your puzzle answer was bjosfbce.

--- Part Two ---

Of course, that would be the message - if you hadn't agreed to use a modified repetition code instead.

In this modified code, the sender instead transmits what looks like random data, but for each character, the character they actually want to send is slightly less likely than the others. Even after signal-jamming noise, you can look at the letter distributions in each column and choose the least common letter to reconstruct the original message.

In the above example, the least common character in the first column is a; in the second, d, and so on. Repeating this process for the remaining characters produces the original message, advent.

Given the recording in your puzzle input and this new decoding methodology, what is the original message that Santa is trying to send?

Your puzzle answer was veqfxzfx.

Both parts of this puzzle are complete! They provide two gold stars: **

At this point, you should return to your advent calendar and try another puzzle.

If you still want to see it, you can get your puzzle input.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void main() {
	ifstream input;
	input.open("Input.txt");
	if (input.is_open()) {
		string line;
		string result = "";

		//Bucket count all the columns separately
		vector<vector<int>> counts(8,vector<int>(26, 0));
		while (getline(input, line)) {
			for (int m = 0; m < line.length(); m++) {
				counts[m][line[m] - 'a']++;
			}
		}


		//PART 1
		//Could use something like the below, but the zero'd counts mess it up
		//result += (char)(max_element(counts[m].begin(), counts[m].end()) - counts[m].begin() + 'a');
		for (int m = 0; m < counts.size(); m++) {
			int max = 0;
			int index = 0;
			for (int n = 0; n < 26; n++) {
				if (counts[m][n] > max) {
					max = counts[m][n];
					index = n;
				}
			}
			result += (char)(index + 'a');
		}
		cout << result << endl;

		//PART 2
		//Could use something like the below, but the zero'd counts mess it up
		//result += (char)(min_element(counts[m].begin(), counts[m].end()) - counts[m].begin() + 'a');
		string result2 = "";
		for (int m = 0; m < counts.size(); m++) {
			int min = INT_MAX;
			int index = 0;
			for (int n = 0; n < 26; n++) {
				if (counts[m][n] != 0 && counts[m][n] < min) {
					min = counts[m][n];
					index = n;
				}
			}
			result2 += (char)(index + 'a');
		}
		cout << result2 << endl;

		input.close();
	}
}