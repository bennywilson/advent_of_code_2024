#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

uint64_t ShiftDecimal(int x) {
	if (x < 10) {
		return 10;
	}
	else if (x < 100) {
		return 100;
	}
	else if (x < 1000) {
		return 1000;
	}
	else if (x < 10000) {
		return 10000;
	}
	else if (x < 100000) {
		return 100000;
	}
	else if (x < 1000000) {
		return 1000000;
	}
	else if (x < 10000000) {
		return 10000000;
	}
	else if (x < 100000000) {
		return 100000000;
	}
}

void generate_ops_recursive(vector<uint32_t>& out_operators, const vector<uint32_t>& set, vector<uint32_t>& subset, const size_t max_len) {
	if (subset.size() == max_len) {
		out_operators.insert(out_operators.end(), subset.begin(), subset.end());
		return;
	}

	for (int i = 0; i < set.size(); i++) {
		subset.push_back(set[i]);
		generate_ops_recursive(out_operators, set, subset, max_len);
		subset.pop_back();
	}
}

/**
 *
 */
#include <chrono>
using namespace std::chrono;

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	vector<int> operands;
	operands.reserve(1000);

	vector<uint32_t> operators;
	operators.reserve(1000);

	vector<uint32_t> subset;
	subset.reserve(1000);

	vector<uint32_t> set;
	set.push_back(0);
	set.push_back(1);
	set.push_back(2);

	string line;
	uint64_t total_calibration = 0;
	while (std::getline(file, line)) {
		operands.resize(0);
		operators.resize(0);
		subset.resize(0);

		stringstream ss(line);

		uint64_t val;
		ss >> val;

		char divider;
		ss >> divider;

		int op;
		while (ss >> op) {
			operands.push_back(op);
		}

		const size_t max_len	 = operands.size() - 1;
		generate_ops_recursive(operators, set, subset, max_len);

		for (size_t i = 0; i < operators.size(); i += max_len) {
			uint64_t total = operands[0];
			for (size_t j = 1; j < operands.size(); j++) {
				const uint64_t totals[] = {
					total + operands[j],
					total * operands[j],
					(total * ShiftDecimal(operands[j])) + operands[j]
				};

				const uint32_t cur_operator = operators[i + j - 1];
				total = totals[cur_operator];
			}

			if (total == val) {
				total_calibration += val;
				break;
			}
		}
	}

	cout << "Total calibration = " << total_calibration << endl;
	return 0;
}
