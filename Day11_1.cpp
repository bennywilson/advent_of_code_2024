#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

uint64_t NumDigits(uint64_t x)
{
	uint64_t num_digits = 0;
	while (x > 0) {
		num_digits++;
		x /= 10;
	}

	return num_digits;
}
/**
 *
 */
int main() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string line;
	uint64_t num;
	list<uint64_t> stones;
	while (file >> num) {
		stones.push_back(num);
		cout << "Pushing " << num << endl;
	}

	for (int blink_num = 0; blink_num < 25; blink_num++) {
		for (auto it = stones.begin(); it != stones.end(); ++it) {
			uint64_t engraving = *it;
			if (engraving == 0) {
				*it = 1;
				continue;
			}

			const uint64_t num_digits = NumDigits(engraving);
			if ((num_digits & 0x1) == 0) {
		//		cout << "Odd detected.  Splitting up " << engraving << endl;
				const uint64_t half_digits = num_digits >> 1;
				uint64_t decimal = 1;
				uint64_t second_half = 0;
				for (int i = 0; i < half_digits; i++) {
					const uint64_t digit = (engraving % 10);
					second_half += digit * decimal;
					decimal = decimal * 10;
					engraving /= 10;
				}
				*it = second_half;
				stones.insert(it, engraving);
			//	cout << "First half is " << engraving << " second half is " << second_half << endl;
			} else {
				*it *= 2024;
			}
		//	cout << "Num digits of " << engraving << " is " << num_digits << endl;
		}
	}


	cout << "Final Output...\n";
	/*for (auto it = stones.begin(); it != stones.end(); ++it) {
		cout << *it << " ";
	}*/
	cout << "\nNum stones is " << stones.size() << endl;
}
// 199753