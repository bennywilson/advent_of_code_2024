#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>

using namespace std;
using namespace std::chrono;

bool g_log = false;


struct Key {
	Key() {}
	int64_t pattern[4];
	bool operator==(const Key& op2) const { return pattern[0] == op2.pattern[0] && pattern[1] == op2.pattern[1] &&
											pattern[2] == op2.pattern[2] && pattern[3] == op2.pattern[3]; }
};

namespace std {
	template<> struct hash<Key> {
		size_t operator()(const Key& k) const {
			size_t res = 
				hash<uint64_t>{}(k.pattern[0]) ^ 
				hash<int64_t>{}(k.pattern[1]) ^ hash<int64_t>{}(k.pattern[2]) ^ hash<int64_t>{}(k.pattern[3]); 
			return res;
		}
	};
};

/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";	

	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	uint64_t total = 0;
	int64_t secret;
	unordered_map<Key, int64_t> sequence_to_sum;

	while (file >> secret) {
		const int64_t starting_secret = secret;
		int64_t last_price = secret % 10;
		vector<int64_t> debug_prices;
		vector<int64_t> price_changes;
		unordered_map<Key, int64_t> local_map;
		for (int64_t secret_iter = 0; secret_iter < 2000; secret_iter++) {
			const int64_t last_secret = secret;
			int64_t op2 = secret * 64;
			secret ^= op2;

			secret %= 16777216;

			op2 = (secret / 32);
			secret &= 0x00000000ffffffff;
			secret ^= op2;

			op2 = secret * 2048;
			secret &= 0x00000000ffffffff;

			secret ^= op2;

			secret %= 16777216;
			const int64_t cur_price = secret % 10;
			int64_t price_delta = cur_price - last_price;
			debug_prices.push_back(cur_price);
			price_changes.push_back(price_delta);
			last_price = cur_price;
		}

		for (size_t i = 3; i < price_changes.size(); i++) {
			Key cur_key;
			cur_key.pattern[0] = price_changes[i - 3];
			cur_key.pattern[1] = price_changes[i - 2];
			cur_key.pattern[2] = price_changes[i - 1];
			cur_key.pattern[3] = price_changes[i - 0];
			if (local_map.find(cur_key) == local_map.end()) {
				local_map[cur_key] = debug_prices[i];
			}
		}

		for (const auto& it: local_map) {
			sequence_to_sum[it.first] += it.second;
		}

		total += secret;
	}
	int64_t largest = 0;
	Key largest_key;
	for (const auto& it : sequence_to_sum) {
		if (it.second > largest) {
			largest = it.second;
			largest_key = it.first;
		}
	}

	cout << "[" << largest_key.pattern[0] << "," << largest_key.pattern[1] << "," << largest_key.pattern[2] << "," << largest_key.pattern[3] << "]: " << largest << endl;


	cout << "Total is " << total;
	return 1;
}


// 2221
