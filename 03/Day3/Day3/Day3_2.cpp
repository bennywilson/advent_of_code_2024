#include <fstream>
#include <string>
#include <iostream>

using namespace std;

bool is_num(const string& inputs, const size_t start, const size_t end) {
	if (start == inputs.npos || end == inputs.npos) {
		return false;
	}

	for (int i = (int)start; i < end; i++) {
		if (inputs[i] < '0' || inputs[i] > '9') {
			return false;
		}
	}
	return true;
}

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string inputs((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	cout << inputs << endl << endl;
	
	size_t cur_index = 0;
	int total = 0;
	size_t next_do = 0;
	size_t next_dont = inputs.find("don't()", 0);
	while (cur_index < inputs.length()) {
		cur_index = inputs.find("mul(", cur_index);
		if (cur_index == inputs.npos) {
			break;
		}

		// Is cur_index in a don't() zone?
		if (next_dont < next_do) {
			if (cur_index > next_do) {
				// cur_index moved into a do() zone, reprocess this mul()
				next_dont = inputs.find("don't()", next_do);
				continue;
			}
			cur_index++;
			continue;
		}

		// Is cur_index in a do() zone?
		if (next_do < next_dont) {
			if (cur_index > next_dont) {
				// cur_index moved into a don't() zone, reprocess this mul()
				next_do = inputs.find("do()", next_dont);
				continue;
			}
		}

		// Get arg 1
		const size_t arg1_start = cur_index + 4;
		cur_index = inputs.find(",", arg1_start);
		if (!is_num(inputs, arg1_start, cur_index)) {
			cur_index = arg1_start;
			continue;
		}

		inputs[cur_index] = '\0';
		const int arg1 = atoi(&inputs[arg1_start]);

		// Get arg 2
		const size_t arg2_start = cur_index + 1;
		cur_index = inputs.find(")", arg2_start);
		if (!is_num(inputs, arg2_start, cur_index)) {
			cur_index = arg2_start;
			continue;
		}

		inputs[cur_index] = '\0';
		const int arg2 = atoi(&inputs[arg2_start]);

		// Multiply!
		total += arg1 * arg2;
		cur_index++;
		cout << "arg1 is " << arg1 << " arg2 is " << arg2 << endl;
	}

	cout << "Total is " << total << endl;
}
