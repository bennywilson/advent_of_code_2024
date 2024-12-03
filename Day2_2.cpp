#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	int num_safe = 0;

	string line;
	while (std::getline(file, line)) {
		// Collect levels from file
		vector<int> levels;
		std::stringstream ss(line);
		int level_val;
		while (ss >> level_val) {
			levels.push_back(level_val);
		}

		// Brute force check
		for (int skip_index = -1; skip_index < (int)levels.size(); skip_index++) {
			bool is_safe = true;
			bool first_run = true;
			int current = 0;
			int inc = 0;

			for (int check_index = 0; check_index < levels.size(); check_index++) {
				if (skip_index == check_index) {
					continue;
				}

				const int prev = current;
				current = levels[check_index];

				if (first_run) {
					first_run = false;
				} else {
					if ((current == prev) ||
						(current > prev && inc < 0) ||
						(current < prev && inc > 0)) {
						is_safe = false;
						break;
					}

					const int dif = abs(current - prev);
					if (dif < 0 || dif > 3) {
						is_safe = false;
						break;
					}

					inc = current - prev;
				}
			}

			if (is_safe) {
				num_safe++;
				break;
			}
		}
	}

	cout << "Num safe is " << num_safe << endl;

	return 1;
}