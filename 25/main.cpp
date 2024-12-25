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
	Schematic(const bool _is_lock) :
		is_lock(_is_lock),
		heights(0){}

	void print() {

		if (is_lock) {
			cout << "#####\n";
		} else {
			cout << ".....\n";
		}

		for (int i = 0; i < 6; i++) {
		
				for (int pin = 0; pin < 5; pin++) {
					int32_t height = heights / (int32_t)pow(10, pin) % 10;
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


	int32_t heights;
	bool is_lock;
};

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	vector<Schematic> keys;
	vector<Schematic> lock;

	string line;
	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		const bool is_lock = line[0] == '#';
		const int32_t height_inc = 1;

		Schematic schematic(is_lock);

		for (int height = 0; height < 6; height++) {
			getline(file, line);
			for (int pin = 0; pin < 5; pin++) {

				if (!is_lock) {
					static int breakhere = 0;
					breakhere++;
				}
				if (line[pin] == '#') {
					const int32_t inc = height_inc * pow(10, pin);
					schematic.heights += inc;
				}
			}
		}
		schematic.print();
		static int breakhere = 0;
		breakhere++;
	}
}

void part_two() { }

/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";	
	part_one();

//	cout << "\n\nPart one..........................................................\n";
	//part_two();//
	return 1;
}

// 56939028423824