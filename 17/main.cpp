#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <sstream>

using namespace std;
using namespace std::chrono;


uint64_t num_digits(uint64_t x) {
	uint64_t digits = 0;
	while (x > 0) {
		digits++;
		x /= 10;
	}

	return digits;
}

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	uint64_t a = 0;
	uint64_t b = 0;
	uint64_t c = 0;
	string line;
	string empty;
	vector<uint32_t> cmds;

	while (std::getline(file, line)) {
		stringstream ss(line);
		ss >> empty >> empty >> a;

		std::getline(file, line);
		ss = stringstream(line);
		ss >> empty >> empty >> b;

		std::getline(file, line);
		ss = stringstream(line);
		ss >> empty >> empty >> c;

		std::getline(file, line);
		std::getline(file, line);

		ss = stringstream(line);
		ss >> empty;

		uint32_t cmd;
		char empty_char;
		while (ss >> cmd) {
			cmds.push_back(cmd);
			ss >> empty_char;
		}
	}

	cout << "Register A: " << a << endl;
	cout << "Register B: " << b << endl;
	cout << "Register C: " << c << endl << endl;
	cout << "Program: ";

	for (const auto cmd : cmds) {
		cout << cmd;
	}
	cout << endl << endl;

	vector<uint64_t> outputs;

	size_t i = 0;
	while (i < cmds.size()) {
		const uint32_t cmd = cmds[i];
		const int64_t op = cmds[i + 1];
		uint64_t combo_op = 0;
		switch (op) {
		case 0: combo_op = 0; break;
		case 1: combo_op = 1; break;
		case 2: combo_op = 2; break;
		case 3: combo_op = 3; break;
		case 4: combo_op = a; break;
		case 5: combo_op = b; break;
		case 6: combo_op = c; break;
		}

		bool jmped = false;

		switch (cmd) {
			// adv
		case 0: { a >>= combo_op; break; }

			// bxl
			case 1: { b ^= op; break; }

			// bst
			case 2: { b = combo_op % 8; break; }

			// jnz
			case 3: {
				if (a != 0) {
					i = op;
					jmped = true;
				}
				break;
			}

			// bxc
			case 4: { b ^= c; break; }

			// out
			case 5: { outputs.push_back(combo_op % 8); break; }

			// bdv
			case 6: { b = a >> combo_op; break; }

			// cdv
			case 7: { c = a >> combo_op; break; }
		}

		if (!jmped) {
			i += 2;
		}
	}

	for (size_t i = 0; i < outputs.size(); i++) {
		cout << outputs[i] << ", ";
	}

	cout << endl;
}

void part_two() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	uint64_t a = 0;
	uint64_t b = 0;
	uint64_t c = 0;
	string line;
	string empty;
	vector<uint32_t> cmds;

	while (std::getline(file, line)) {
		stringstream ss(line);
		ss >> empty >> empty >> a;

		std::getline(file, line);
		ss = stringstream(line);
		ss >> empty >> empty >> b;

		std::getline(file, line);
		ss = stringstream(line);
		ss >> empty >> empty >> c;

		std::getline(file, line);
		std::getline(file, line);

		ss = stringstream(line);
		ss >> empty;

		uint32_t cmd;
		char empty_char;
		while (ss >> cmd) {
			cmds.push_back(cmd);
			ss >> empty_char;
		}
	}

	cout << "Register A: " << a << endl;
	cout << "Register B: " << b << endl;
	cout << "Register C: " << c << endl << endl;
	cout << "Program: ";

	for (const auto cmd : cmds) {
		cout << cmd;
	}
	cout << endl << endl;

	vector<uint64_t> next_a_list_1;
	next_a_list_1.push_back(0);

	vector<uint64_t> next_a_list_2;

	for (int64_t cur_digit = cmds.size() - 1; cur_digit >= 0; cur_digit--) {
		cout << "Iteration " << cmds.size() - cur_digit - 1 << ".........................\n";

		vector<uint64_t>& src_next_a = (next_a_list_1.size() > 0) ? (next_a_list_1):(next_a_list_2);
		vector<uint64_t>& dst_next_a = (next_a_list_1.size() > 0) ? (next_a_list_2) : (next_a_list_1);

		for (size_t idx = 0; idx < src_next_a.size(); idx++) {
			cout << "	idx " << src_next_a[idx] << endl;
			for (uint64_t check_val = 0; check_val < 8; check_val++) {
				vector<uint64_t> outputs;
				a = (src_next_a[idx] << 3) + check_val;
				cout << "		a = " << a << endl;
				uint64_t start_a = a;

				size_t i = 0;
				while (i < cmds.size()) {
					const uint32_t cmd = cmds[i];
					const uint64_t op = cmds[i + 1];
					uint64_t combo_op = 0;
					switch (op) {
						case 0: combo_op = 0; break;
						case 1: combo_op = 1; break;
						case 2: combo_op = 2; break;
						case 3: combo_op = 3; break;
						case 4: combo_op = a; break;
						case 5: combo_op = b; break;
						case 6: combo_op = c; break;
					}

					bool jmped = false;
					switch (cmd) {
						// adv
						case 0: { a >>= combo_op; break; }

						// bxl
						case 1: { b ^= op; break; }

						// bst
						case 2: { b = combo_op % 8; break; }

						// jnz
						case 3: { if (a != 0) { i = op; jmped = true; } break; }

						// bxc
						case 4: { b ^= c; break; }

						// out
						case 5: { outputs.push_back(combo_op % 8); break; }

						// bdv
						case 6: { b = a >> combo_op; break; }

						// cdv
						case 7: { c = a >> combo_op; break; }
					}
					if (!jmped) {
						i += 2;
					}
				}

				if (outputs.size() == cmds.size() - cur_digit) {
					bool is_match = true;
					for (int j = 0; j < outputs.size(); j++) {
						if (outputs[j] != cmds[cur_digit + j]) {
							is_match = false;
							break;
						}
					}

					if (is_match) {
						dst_next_a.push_back(start_a);
						cout << "			>>> Pushing back <<< " << start_a << endl;
					}
				}
			} // While cmds
		}	 // Perm loop
	
		//	getchar();
		if (dst_next_a.size() > 0) {
			src_next_a.clear();
		} else {
			cout << "Infinite loop detected!" << endl;
		}
	}

	vector<uint64_t>& src_next_a = (next_a_list_1.size() > 0) ? (next_a_list_1) : (next_a_list_2);
	cout << "Lowest initialization value = " << src_next_a[0];
}

/**
 *
 */
int main() {
	cout << "Part one...\n";
	part_one();

	getchar();

	cout << "Part two...\n";
	part_two();

}

// 3696066995494
// 3696066995494 -- Too low
// 236548287712959
///236548287712877