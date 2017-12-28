/*
Note: I was so close to actually being a top 100 this time. By less than a minute for part 1 and 5 minutes for part 2
This one had more to do with figuring out WHAT happens rather than brute forcing it. The first part was easy to find,
the second part took a little work. Was nice to implement a linked list though.

Rank
Part 1: 110
Part 2: 124

--- Day 19: An Elephant Named Joseph ---

The Elves contact you over a highly secure emergency channel. Back at the North Pole, the Elves are busy misunderstanding White Elephant parties.

Each Elf brings a present. They all sit in a circle, numbered starting with position 1. Then, starting with the first Elf, they take turns stealing all the presents from the Elf to their left. An Elf with no presents is removed from the circle and does not take turns.

For example, with five Elves (numbered 1 to 5):

1
5   2
4 3
Elf 1 takes Elf 2's present.
Elf 2 has no presents and is skipped.
Elf 3 takes Elf 4's present.
Elf 4 has no presents and is also skipped.
Elf 5 takes Elf 1's two presents.
Neither Elf 1 nor Elf 2 have any presents, so both are skipped.
Elf 3 takes Elf 5's three presents.
So, with five Elves, the Elf that sits starting in position 3 gets all the presents.

With the number of Elves given in your puzzle input, which Elf gets all the presents?

Your puzzle answer was 1808357.

--- Part Two ---

Realizing the folly of their present-exchange rules, the Elves agree to instead steal presents from the Elf directly across the circle. If two Elves are across the circle, the one on the left (from the perspective of the stealer) is stolen from. The other rules remain unchanged: Elves with no presents are removed from the circle entirely, and the other elves move in slightly to keep the circle evenly spaced.

For example, with five Elves (again numbered 1 to 5):

The Elves sit in a circle; Elf 1 goes first:
1
5   2
4 3
Elves 3 and 4 are across the circle; Elf 3's present is stolen, being the one to the left. Elf 3 leaves the circle, and the rest of the Elves move in:
1           1
5   2  -->  5   2
4 -          4
Elf 2 steals from the Elf directly across the circle, Elf 5:
1         1
-   2  -->     2
4         4
Next is Elf 4 who, choosing between Elves 1 and 2, steals from Elf 1:
-          2
2  -->
4          4
Finally, Elf 2 steals from Elf 4:
2
-->  2
-
So, with five Elves, the Elf that sits starting in position 2 gets all the presents.

With the number of Elves given in your puzzle input, which Elf now gets all the presents?

Your puzzle answer was 1407007.

Both parts of this puzzle are complete! They provide two gold stars: **

At this point, you should return to your advent calendar and try another puzzle.

Your puzzle input was 3001330.
*/
#include <iostream>

using namespace std;

struct elf {
	elf *next;
	int n;
};

void main() {
	//PART 1
	int input1 = 3001330;
	int stealingElf = 1;
	int counter = 1;

	//This part all depends on if the input is even of odd.
	//Even input removes every other elf 
	//Odd input removes every other elf AND the first elf 
	while (input1 != 1) {
		if (input1 % 2 == 1) {
			//Change the first elf based on how many circles we have made, doubling each time (2, 4, 8, 16, etc)
			stealingElf += 2 * counter;
		}
		input1 /= 2;
		counter*=2;
	}
	cout << "Part 1 Answer : " << stealingElf << endl;


	//PART 2
	//The solution uses a circular linked list and then progresses through 
	//keeping every THIRD node and discarding the other 2, starting at a specific index
	//until only one is left
	long long int input2 = 3001330;
	elf *head = new elf();
	elf *currElf = head;
	elf *prevElf;
	head->n = 1;
	head->next = NULL;

	//Set up a circular linked list to act as the circle of elves
	for (int m = 2; m <= input2; m++) {
		elf *newElf = new elf();
		newElf->n = m;
		newElf->next = head;

		currElf->next = newElf;
		prevElf = currElf;
		currElf = currElf->next;
	}
	currElf = head;
	prevElf = head;

	//Get the first elf to be deleted, altering it by one if the input is even
	int target = input2 / 2;
	if (input2 % 2 == 0)
		target--;
	for (int m = 0; m < target; m++) {
		prevElf = currElf;
		currElf = currElf->next;
	}

	while (input2 > 1) {
		//An even input skips one elf while an odd input deletes the very next elf
		if (input2 % 2 == 0) {
			prevElf = currElf;
			currElf = currElf->next;
		}
		prevElf->next = currElf->next;
		delete currElf;
		currElf = prevElf->next;
		input2--;
	}
	cout << "Part 2 Answer : " << currElf->n << endl;
}