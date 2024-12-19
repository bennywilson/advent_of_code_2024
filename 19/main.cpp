#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

using namespace std;
using namespace std::chrono;

bool g_log = false;

template<typename V,typename T>
bool std_find(const vector<V>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template<typename V, typename T>
bool std_find(const V& vec, const T& value) {
	return vec.find(value) != vec.end();
}

bool has_match(string design, unordered_set<string>& patterns, unordered_map<string, bool>& cache, string& tabs) {
	if (std_find(cache, design)) {
		return cache[design];
	}

	if (std_find(patterns, design)) {
		return true;
	}

	if (design.size() == 1) {
		return false;
	}

	for (size_t l = design.length() - 1; l >= 1; l--) {
		string first = design.substr(0, l);
		string second = design.substr(l);

		tabs += "    ";
		if (has_match(first, patterns, cache, tabs) && has_match(second, patterns, cache, tabs)) {
			tabs.resize(tabs.size() - 3);
			cache[design] = true;
			return true;
		}
	}

	tabs.resize(tabs.size() - 3);
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
			if (!std_find(patterns, pattern)) {
				patterns.push_back(pattern);
				pattern_set.insert(pattern);
			}
		}
	}

	vector<string> designs;
	while (std::getline(file, line)) {
		designs.push_back(line);
	}

	if (g_log) {
		cout << "\nPatterns: ";
		for (size_t i = 0; i < patterns.size(); i++) {
			cout << patterns[i] << " ";
		}

		cout << "\n\nDesigns:\n";
		for (size_t i = 0; i < designs.size(); i++) {
			cout << designs[i] << " ";
		}
	}

	if (g_log) {
		cout << "\n\n\nRunning program_____" << endl;
	}

	uint64_t num_found = 0;
	for (int i = 0; i < designs.size(); i++) {
		cout << designs[i] << " ";
		unordered_map<string, bool> cache;
		string tabs;
		if (has_match(designs[i], pattern_set, cache, tabs)) {
			num_found++;	
			if (g_log) {
				cout << "	Design " << designs[i] << "has a match" << endl << endl;
			}
		}
	}
}

uint64_t count_matches(string design, const uint64_t cur_idx, unordered_set<string>& patterns, unordered_map<string, uint64_t>& cache, string& tabs) {
	if (std_find(cache, design)) {
		if (cache[design] > 0) {
			if (g_log) {
				cout << tabs << " Cached design " << design << " match found! " << endl;
			}
		}
		return cache[design];
	}

	uint64_t count = 0;
	for (size_t i = 1; i < design.size(); i++) {
		const string first = design.substr(0, i);
		if (!std_find(patterns, first)) {
			continue;
		}

		const string second = design.substr(i);
		if (std_find(patterns, second)) {
			if (g_log) {
				cout << tabs << "Match found!" << endl;
			}
			count++;
		}

		if (g_log) {
			cout << tabs << first << ", " << second << endl;
		}

		tabs += "   ";
		count += count_matches(second, 0, patterns, cache, tabs);
		tabs.resize(tabs.size() - 3);
	}

	cache[design] = count;
	//tabs.resize(tabs.size() - 3);

	if (g_log && design.size() == 1) {
		cout << endl;
	}

	return count;
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
			if (!std_find(patterns, pattern)) {
				patterns.push_back(pattern);
				pattern_set.insert(pattern);
			}
		}
	}

	vector<string> designs;
	while (std::getline(file, line)) {
		designs.push_back(line);
	}
	unordered_map<string, uint64_t> cache;
	uint64_t num_found = 0;

	string tabs;
	for (const auto& design: designs) {
		num_found += count_matches(design, 0, pattern_set, cache, tabs);
	}

	cout << "Num found is " << num_found << endl;
}


/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";
	part_one();

	cout << "\n\nPart two......................................................\n";
	part_two();

}
