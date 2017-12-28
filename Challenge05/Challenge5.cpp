/*
NOTE:
Once again, C++ lumbers slowly to the finish line. C++ was super annoying for this. 
Whereas other languages have built in fast md5, I had to pull an md5 from 
a website. Then it took ages to hash billions of times... Probably could have found
a better md5 implementation. Also could have remembered to write down WHAT and WHERE the important hashes were
so I didn't have to REPEATEDLY START FROM THE BEGINNING.

Rank:
Part 1: 668
Part 2: 660

--- Day 5: How About a Nice Game of Chess? ---

You are faced with a security door designed by Easter Bunny engineers that seem to have acquired most of their security knowledge by watching hacking movies.

The eight-character password for the door is generated one character at a time by finding the MD5 hash of some Door ID (your puzzle input) and an increasing integer index (starting with 0).

A hash indicates the next character in the password if its hexadecimal representation starts with five zeroes. If it does, the sixth character in the hash is the next character of the password.

For example, if the Door ID is abc:

The first index which produces a hash that starts with five zeroes is 3231929, which we find by hashing abc3231929; the sixth character of the hash, and thus the first character of the password, is 1.
5017308 produces the next interesting hash, which starts with 000008f82..., so the second character of the password is 8.
The third time a hash starts with five zeroes is for abc5278568, discovering the character f.
In this example, after continuing this search a total of eight times, the password is 18f47a30.

Given the actual Door ID, what is the password?

Your puzzle answer was c6697b55.

The first half of this puzzle is complete! It provides one gold star: *

--- Part Two ---

As the door slides open, you are presented with a second door that uses a slightly more inspired security mechanism. Clearly unimpressed by the last version (in what movie is the password decrypted in order?!), the Easter Bunny engineers have worked out a better solution.

Instead of simply filling in the password from left to right, the hash now also indicates the position within the password to fill. You still look for hashes that begin with five zeroes; however, now, the sixth character represents the position (0-7), and the seventh character is the character to put in that position.

A hash result of 000001f means that f is the second character in the password. Use only the first result for each position, and ignore invalid positions.

For example, if the Door ID is abc:

The first interesting hash is from abc3231929, which produces 0000015...; so, 5 goes in position 1: _5______.
In the previous method, 5017308 produced an interesting hash; however, it is ignored, because it specifies an invalid position (8).
The second interesting hash is at index 5357525, which produces 000004e...; so, e goes in position 4: _5__e___.
You almost choke on your popcorn as the final character falls into place, producing the password 05ace8e3.

Given the actual Door ID and this new method, what is the password? Be extra proud of your solution if it uses a cinematic "decrypting" animation.

Your puzzle input is still ffykfhsq.

Answer:
8c35d1ab

You can also [Share] this puzzle.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

//http://www.zedwood.com/article/cpp-md5-function
#include "md5.h"

using namespace std;


void main() {
	//ifstream input;
	string input = "ffykfhsq";
	string hash;
	string password = "        ";
	int count = 0;

	for (int m = 0; m < 100000000; m++) {
		//Log so we know we are still running
		if ((m % 100000 == 0))
			cout << m << endl;

		string currentString = input + to_string(m);
		hash = md5(currentString);
		if(hash.compare(0, 5, "00000") == 0){

			//Log that we found an interesting hash
			cout << "HASH at " << m << ": " << hash << endl;

			//PART 1
			//password += hash[5];

			//PART 2
			//Check if the location is valid and that the location in the password is not taken yet
			if (hash[5] < '8' && hash[5] >= '0' && password[hash[5] - '0'] == ' ') {
				count++;
				password.replace(hash[5]-'0', 1, 1, hash[6]);

				//Log so we know things are getting replaced properly
				cout << "REPLACING !" << endl;;
				cout << password << endl;
			}

			//Stop if we found the password
			if (count == 8)
				break;
		}
	}

	cout << password << endl;

	//Reminders of where indexes were found (the number is AFTER the hash was found, nearest 100000)
	//8c 5d1ab
	//24100000

	//8c35d1ab
	//26400000
}