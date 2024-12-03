#include <fstream>
#include <string>
#include <iostream>

using namespace std;

bool is_num(const string& inputs, size_t start, size_t end) {
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
	while (cur_index < inputs.length()) {
		cur_index = inputs.find("mul(", cur_index);
		if (cur_index == inputs.npos) {
			break;
		}
		
		const size_t start_index = cur_index + 4;

		// Get arg 1
		const size_t comma = inputs.find(",", start_index);
		cur_index = comma;
		if (!is_num(inputs, start_index, comma)) {
			cur_index = start_index;
			continue;
		}

		inputs[comma] = '\0';
		int arg1 = atoi(&inputs[start_index]);

		// Get arg 2
		const size_t end_paren = inputs.find(")", cur_index);
		if (!is_num(inputs, comma + 1, end_paren)) {
			cur_index = comma + 1;
			continue;
		}

		inputs[end_paren] = '\0';
		int arg2 = atoi(&inputs[comma + 1]);

		// Multiply!
		total += arg1 * arg2;
		cur_index = end_paren + 1;
		cout << "arg1 is " << arg1 << " arg2 is " << arg2 << endl;
	}

	cout << "Total is " << total << endl;
}
