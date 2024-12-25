#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template<typename V, typename T>
bool std_contains(const vector<V>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template<typename V, typename T>
bool std_contains(const V& vec, const T& value) {
	return vec.find(value) != vec.end();
}

struct Schematic {
	void print() {
		if (is_lock) { cout << "#####\n"; } else { cout << ".....\n"; }

		for (int i = 0; i < 6; i++) {
				for (int pin = 0; pin < 5; pin++) {
					int32_t height = pin_height(pin);
					if (!is_lock) height = 6 - height;
					if (height > i ) {
						if (is_lock) cout << "#"; else cout << ".";
					} else {
						if (is_lock) cout << "."; else cout << "#";
					}
				}
				cout << endl;
		
		}
		cout << endl;
	}

	int32_t pin_height(int32_t pin) {
		return heights / (int32_t)pow(10, pin) % 10;
	}

	int32_t heights;
	bool is_lock;
};

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	Schematic* root = nullptr;

	vector<Schematic*> keys;
	vector<Schematic*> locks;

	string line;
	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		const bool is_lock = line[0] == '#';
		const int32_t end = (is_lock)?(6):(5);

		Schematic* const schematic = new Schematic();
		schematic->is_lock = is_lock;
		for (int height = 0; height < 6; height++) {
			getline(file, line);
			if (height == 5 && !is_lock) break;
			for (int pin = 0; pin < 5; pin++) {
				if (line[pin] == '#') {
					const int32_t inc = (int32_t)pow(10, pin);
					schematic->heights += inc;
				}
			}
		}
		//schematic->print();

		if (is_lock) {
			locks.push_back(schematic);
		} else {
			keys.push_back(schematic);
		}

		static int breakhere = 0;
		breakhere++;
	}

	uint64_t total_matches = 0;
	for (int k = 0; k < keys.size(); k++) {
		for (int l = 0; l < locks.size(); l++) {
			if (keys[k]->pin_height(0) + locks[l]->pin_height(0) <= 5 && 
				keys[k]->pin_height(1) + locks[l]->pin_height(1) <= 5 && 
				keys[k]->pin_height(2) + locks[l]->pin_height(2) <= 5 && 
				keys[k]->pin_height(3) + locks[l]->pin_height(3) <= 5 &&
				keys[k]->pin_height(4) + locks[l]->pin_height(4) <= 5) {
				total_matches++;
			} else {
			}
		}
	}

	cout << "\nThere are " << total_matches << " total matches. " << endl;
}

void part_two() { }

/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";	
	part_one();
	return 1;
}