#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace std::chrono;

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

void generate_ops_recursive(vector<uint8_t>& out_operators, const vector<uint8_t>& set, vector<uint8_t>& subset, const size_t max_len) {
	if (subset.size() == max_len) {
		out_operators.insert(out_operators.end(), subset.begin(), subset.end());
		return;
	}

	for (size_t i = 0; i < set.size(); i++) {
		subset.push_back(set[i]);
		generate_ops_recursive(out_operators, set, subset, max_len);
		subset.pop_back();
	}
}
stack<int8_t> set_idx_stack;

void generate_ops_iterative(vector<uint8_t>& out_operators, const vector<uint8_t>& set, vector<uint8_t>& subset, const size_t max_len) {
	set_idx_stack.push(-1);
	while (!set_idx_stack.empty()) {
		if (subset.size() == max_len) {
			out_operators.insert(out_operators.end(), subset.begin(), subset.end());
			subset.pop_back();
			set_idx_stack.pop();
		}
		else {
			int8_t& set_idx = set_idx_stack.top();
			set_idx++;
			if (set_idx < (int8_t)set.size()) {
				subset.push_back(set[set_idx]);
				set_idx_stack.push(-1);
			}
			else {
				if (subset.size() > 0) {
					subset.pop_back();
				}
				set_idx_stack.pop();
			}

		}
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

		auto start = high_resolution_clock::now();

		vector<int> operands;
		operands.reserve(50000);

		vector<uint8_t> operators;
		operators.reserve(50000);

		vector<uint8_t> subset;
		subset.reserve(50000);

		vector<uint8_t> set;
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

			const size_t max_len = operands.size() - 1;

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
					//	cout << "Found!\n";
					total_calibration += val;
					break;
				}
			}
		}

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Total calibration = " << total_calibration << ". Duration " << duration.count() << endl;
		return 0;
	}
