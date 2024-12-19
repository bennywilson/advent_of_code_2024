#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <windows.h>
#include <conio.h>

using namespace std;
using namespace std::chrono;

template<typename V, typename T>
bool std_contains(const V& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template<typename V, typename T>
bool map_contains(const V& vec, const T& value) {
	return vec.find(value) != vec.end();
}


string tabs;

bool has_match(string design, unordered_set<string>& patterns, unordered_map<string, bool>& cache) {
	if (map_contains(cache, design)) {
		return cache[design];
	}

	if (std_contains(patterns, design)) {
		return true;
	}

	if (design.size() == 1) {
		return false;
	}

	for (size_t l = design.length() - 1; l >= 1; l--) {
		string first = design.substr(0, l);
		string second = design.substr(l);

		tabs += "    ";
		if (has_match(first, patterns, cache) && has_match(second, patterns, cache)) {
			tabs.pop_back();
			tabs.pop_back();
			tabs.pop_back();
			cache[design] = true;
			return true;
		}
	}

	tabs.pop_back();
	tabs.pop_back();
	tabs.pop_back();
	cache[design] = false;
	return false;
}

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	vector<string> patterns;
	unordered_set<string> pattern_set;

	while (std::getline(file, line) && line.size() > 0) {
		stringstream ss(line);
		string pattern;
		while (ss >> pattern) {
			if (pattern.back() == ',') {
				pattern = pattern.substr(0, pattern.length() - 1);
			}
			if (!std_contains(patterns, pattern)) {
				//cout << "Pushing " << pattern << endl;
				patterns.push_back(pattern);
				pattern_set.insert(pattern);
			}
		}
	}

	vector<string> designs;
	while (std::getline(file, line)) {
		designs.push_back(line);
	}

	cout << "\nPatterns: ";
	for (size_t i = 0; i < patterns.size(); i++) {
		cout << patterns[i] << " ";
	}

	cout << "\n\nDesigns:\n";
	for (size_t i = 0; i < designs.size(); i++) {
		cout << designs[i] << endl;
	}

	uint64_t num_found = 0;
	for (int i = 0; i < designs.size(); i++) {
		cout << designs[i] << " ";
		unordered_map<string, bool> cache;
		if (has_match(designs[i], pattern_set, cache)) {
			num_found++;
			//cout << " was found!";
		}
		else {
		//	cout << " was not found!";
		}

	//	cout << endl;
	}

	cout << "Num found is " << num_found << endl;
}

bool count_matches(string design, unordered_set<string>& patterns, unordered_map<string, bool>& cache) {
	if (map_contains(cache, design)) {
		return cache[design];
	}

	if (std_contains(patterns, design)) {
		return true;
	}

	if (design.size() == 1) {
		return false;
	}

	for (size_t l = design.length() - 1; l >= 1; l--) {
		string first = design.substr(0, l);
		string second = design.substr(l);

		tabs += "    ";
		if (count_matches(first, patterns, cache) && count_matches(second, patterns, cache)) {
			tabs.pop_back();
			tabs.pop_back();
			tabs.pop_back();
			cache[design] = true;
			return true;
		}
	}

	tabs.pop_back();
	tabs.pop_back();
	tabs.pop_back();
	cache[design] = false;
	return false;
}

void part_two() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	vector<string> patterns;
	unordered_set<string> pattern_set;

	while (std::getline(file, line) && line.size() > 0) {
		stringstream ss(line);
		string pattern;
		while (ss >> pattern) {
			if (pattern.back() == ',') {
				pattern = pattern.substr(0, pattern.length() - 1);
			}
			if (!std_contains(patterns, pattern)) {
				//cout << "Pushing " << pattern << endl;
				patterns.push_back(pattern);
				pattern_set.insert(pattern);
			}
		}
	}

	vector<string> designs;
	while (std::getline(file, line)) {
		designs.push_back(line);
	}

	cout << "\nPatterns: ";
	for (size_t i = 0; i < patterns.size(); i++) {
		cout << patterns[i] << " ";
	}

	cout << "\n\nDesigns:\n";
	for (size_t i = 0; i < designs.size(); i++) {
		cout << designs[i] << endl;
	}

		uint64_t num_found = 0;
		for (int i = 0; i < designs.size(); i++) {
			cout << designs[i] << " ";
			unordered_map<string, bool> cache;
			if (has_match(designs[i], pattern_set, cache)) {
				num_found++;
				cout << " was found!";
			}
			else {
				cout << " was not found!";
			}

			cout << endl;
		}

	cout << "Num found is " << num_found << endl;
}


/**
 *
 */
int main() {
	cout << "Part one...\n";
	part_one();

	cout << "\n\nPart two ...\n";
	part_two();

}

// 278