#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <windows.h>
#include <conio.h>

using namespace std;
using namespace std::chrono;

template<typename T>
bool vec_contains(const vector<T>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	vector<string> patterns;
	while (std::getline(file, line) && line.size() > 0) {
		stringstream ss(line);
		string pattern;
		while (ss >> pattern) {
			if (pattern.back() == ',') {
				pattern = pattern.substr(0, pattern.length() - 1);
			}
			if (!vec_contains(patterns, pattern)) {
				//cout << "Pushing " << pattern << endl;
				patterns.push_back(pattern);
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

}

/**
 *
 */
int main() {
	cout << "Part one...\n";
	part_one();
}
