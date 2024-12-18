#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace std::chrono;

/**
 *
 */
int main() {
	const auto start = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	uint32_t check_sum = 0;
	string line;
	while (std::getline(file, line)) {
		cout << line << endl;


		vector<uint16_t> disk_map;
		for (uint32_t i = 0; i < line.size(); i++) {
			uint16_t output = UINT16_MAX;
			if (!(i & 0x01)) {
				output = i >> 1;
			}

			const uint32_t count = line[i] - '0';
			for (uint32_t j = 0; j < count; j++) {
				disk_map.push_back(output);
			}
		}

		for (int i = 0; i < disk_map.size(); i++) {
			if (disk_map[i] == UINT16_MAX) {
				cout << "." << " ";
			} else {
				cout << disk_map[i] << " ";
			}
		}
		cout << endl;

		size_t start = 0;
		size_t end = disk_map.size() - 1;

		while (start <= end) {
			while (start < disk_map.size() && disk_map[start] != UINT16_MAX) {
				start++;
			}


			while (end != UINT16_MAX && disk_map[end] == UINT16_MAX) {
				end--;
			}

			if (end < UINT16_MAX && start < end) {
				const uint16_t swap_val = disk_map[start];
				disk_map[start] = disk_map[end];
				disk_map[end] = swap_val;
			}
		}


		for (int i = 0; i < disk_map.size(); i++) {
			if (disk_map[i] == UINT16_MAX) {
				cout << "." << " ";
			}
			else {
				cout << disk_map[i] << " ";
			}
		}
		cout << endl;
		const uint32_t line_len = (uint32_t)line.size();
		for (uint32_t i = 0; i < line_len && line[i] != '.'; i++) {
			cout << i << " * " << disk_map[i] <<  " = " << i * disk_map[i] << endl;
			check_sum += i * disk_map[i];
		}
		cout << line << endl;
	}

	cout << "Check sum is " << check_sum << endl;
}

// 301