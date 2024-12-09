#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;


void generate_ops_recursive(vector<uint32_t>& out_operators, const vector<uint32_t>& set, vector<uint32_t>& subset, int max_len) {
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
int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	vector<int> operands;
	operands.reserve(5000);

	vector<uint32_t> operators;
	operators.reserve(5000);

	vector<uint32_t> subset;
	subset.reserve(5000);

	vector<uint32_t> set;
	set.push_back(0);
	set.push_back(1);

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

		const size_t max_len = operands.size() - 1;
		generate_ops_recursive(operators, set, subset, max_len);

		for (size_t i = 0; i < operators.size(); i += max_len) {
			uint64_t total = operands[0];
			for (size_t j = 1; j < operands.size(); j++) {
				const uint32_t cur_operator = operators[i + j - 1];
				if (cur_operator == 0) {
					total = total + operands[j];
				} else {
					total = total * operands[j];
				}
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
