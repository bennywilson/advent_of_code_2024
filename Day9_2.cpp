#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
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

	uint64_t check_sum = 0;
	string line;

	while (std::getline(file, line)) {
		cout << line << endl;

		struct FreeBlockInfo {
			FreeBlockInfo(int in_index, int in_numfree) :
				index(in_index),
				num_free(in_numfree) {}
			int index;
			int num_free;
		};
		list<FreeBlockInfo> free_blocks;
		vector<uint16_t> disk_map;
		for (uint32_t i = 0; i < line.size(); i++) {
			uint16_t output = UINT16_MAX;
			const uint32_t count = line[i] - '0';
			if (!(i & 0x01)) {
				output = i >> 1;
			} else {
				FreeBlockInfo new_block(disk_map.size(), count);
				free_blocks.push_back(new_block);
			}

			for (uint32_t j = 0; j < count; j++) {
				disk_map.push_back(output);
			}
		}

		for (auto it = free_blocks.begin(); it != free_blocks.end(); ++it) {
			cout << it->index << " - " << it->num_free << endl;
		}

		size_t start = 0;
		size_t end = disk_map.size() - 1;
		const size_t max_size = (size_t)-1;
		for (auto it = disk_map.begin(); it != disk_map.end(); ++it) {
			if (*it == UINT16_MAX) {
				cout << ".";
			}
			else {
				cout << *it;
			}
		}

		while (start <= end) {
			while (start < disk_map.size() && disk_map[start] != UINT16_MAX) {
				start++;
			}

			while (end != max_size && disk_map[end] == UINT16_MAX) {
				end--;
			}

			if (end < max_size && start < end) {
				const uint16_t swap_val = disk_map[start];
				disk_map[start] = disk_map[end];
				disk_map[end] = swap_val;
			}
		}


		for (auto it = disk_map.begin(); it != disk_map.end(); ++it) {
			if (*it == UINT16_MAX) {
				cout << ".";
			} else {
				cout << *it;
			}
		}
		cout << endl;

		for (uint32_t i = 0; i < (uint32_t)disk_map.size(); i++) {
			if (disk_map[i] == UINT16_MAX) {
				break;
			}
			check_sum += (uint64_t)i * disk_map[i];
		}
	}

	cout << "Check sum is " << check_sum << endl;
}
// 6283170117911