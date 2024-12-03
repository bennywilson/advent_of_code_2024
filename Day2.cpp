#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	int num_safe = 0;

	string line;
	while (std::getline(file, line)) {
		int prev;
		std::stringstream ss(line);
		if (ss >> prev) {
			bool is_safe = true;
			int inc = 0;
			int current;
			while (ss >> current) {
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
				prev = current;
			}
			if (is_safe) {
				num_safe++;
			}
		}
	}

	cout << "Num safe is " << num_safe << endl;

	return 1;
}