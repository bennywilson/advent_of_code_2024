#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>

using namespace std;
using namespace std::chrono;

uint64_t NumDigits(uint64_t x) {
	uint64_t num_digits = 0;
	while (x > 0) {
		num_digits++;
		x /= 10;
	}

	return num_digits;
}

void part_one() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
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
				continue;
			}

			*it *= 2024;
		}
	}


	cout << "Final Output...\n";
	/*for (auto it = stones.begin(); it != stones.end(); ++it) {
		cout << *it << " ";
	}*/
	cout << "\nNum stones is " << stones.size() << endl;
}

void part_two() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	uint64_t num;
	map<uint64_t, uint64_t> stones_1;
	map<uint64_t, uint64_t> stones_2;

	while (file >> num) {
		stones_1[num]++;
	}

	for (int blink_num = 0; blink_num < 75; blink_num++) {
		const map<uint64_t, uint64_t>& src = (blink_num & 0x01) ? (stones_2) : (stones_1);
		map<uint64_t, uint64_t>& dst = (blink_num & 0x01) ? (stones_1) : (stones_2);
		dst.clear();

		for (auto it = src.begin(); it != src.end(); ++it) {
			uint64_t engraving = it->first;
			if (engraving == 0) {
				dst[1] += it->second;
				continue;
			}

			const uint64_t num_digits = NumDigits(engraving);
			if ((num_digits & 0x1) == 0) {
				const uint64_t half_digits = num_digits >> 1;
				uint64_t decimal = 1;
				uint64_t second_half = 0;
				for (int i = 0; i < half_digits; i++) {
					const uint64_t digit = (engraving % 10);
					second_half += digit * decimal;
					decimal = decimal * 10;
					engraving /= 10;
				}
				dst[engraving] += it->second;
				dst[second_half] += it->second;
				continue;
			}

			dst[engraving * 2024] += it->second;
		}
	}

	const auto end_time = high_resolution_clock::now();
	std::chrono::milliseconds elapsed = duration_cast<std::chrono::milliseconds>(end_time - start_time);
	uint64_t total = 0;
	const map<uint64_t, uint64_t>& src = (stones_1.size() > stones_2.size()) ? (stones_1) : (stones_2);
	for (auto it = src.begin(); it != src.end(); ++it) {
		total += it->second;
	}
	cout << "\nNum stones is " << total << " elapsed time is " << elapsed.count() << endl;
}

/**
 *
 */
int main() {
	part_one();
	part_two();
}
// 199753