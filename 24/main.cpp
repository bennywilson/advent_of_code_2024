#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template<typename V, typename T>
bool std_contains(const vector<V>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template<typename V, typename T>
bool std_contains(const V& vec, const T& value) {
	return vec.find(value) != vec.end();
}

struct Gate {
	Gate() {}
	string op1;
	string op2;
	string operand;
	string out;

	void operator=(const Gate& op) {
		op1 = op.op1;
		op2 = op.op2;
		operand = op.operand;
		out = op.out;
	}

	bool operator=(const Gate& op) const { return op1 == op.op1 && op2 == op.op2 && out == op.out && operand == op.operand; }
};

namespace std {
	template<> struct hash<Gate> {
		size_t operator()(const Gate& r) const {
			size_t res = hash<string>{}(r.op1) ^ hash<string>{}(r.op2) ^ hash<string>{}(r.out);
			return res;
		}
	};
};

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	unordered_map<string, uint8_t> wire_to_val;
 
	// Collect starting values for wires
	string line;
	getline(file, line);

	while (!line.empty()) {
		const string& wire_name = line.substr(0, 3);
		const uint8_t wire_value = atoi(&line[5]);
		wire_to_val[wire_name] = wire_value;
		getline(file, line);

		cout << wire_name << ": " << wire_value << endl;
	}

	// Build gates
	vector<Gate> gates;
	uint64_t zbit_count = 0;

	while (getline(file, line)) {
		stringstream ss(line);
		string extra;
		Gate new_gate;
		ss >> new_gate.op1 >> new_gate.operand >> new_gate.op2 >> extra >> new_gate.out;
		gates.push_back(new_gate);

		if (new_gate.out[0] == 'z') {
			const uint64_t z_idx = atoi(new_gate.out.substr(1).c_str()) + (uint64_t)1;
			if (z_idx > zbit_count) {
				zbit_count = z_idx;
			}
		}
	}
	cout << "z has " << zbit_count << " bits" << endl;

	uint64_t zbits_written = 0;
	while (zbits_written < zbit_count) {
		for (int64_t i = gates.size() - 1; i >= 0; i--) {
			const Gate& gate = gates[i];
			if (!std_contains(wire_to_val, gate.op1) ||
				!std_contains(wire_to_val, gate.op2)) {
				continue;
			}
			
			assert(!std_contains(wire_to_val, gate.out));

			uint8_t out;
			uint8_t op1 = wire_to_val[gate.op1], op2 = wire_to_val[gate.op2];
			if (gate.operand == "AND") {
				out = wire_to_val[gate.op1] & wire_to_val[gate.op2];
			} else if (gate.operand == "OR") {
				out = wire_to_val[gate.op1] | wire_to_val[gate.op2];
			} else if (gate.operand == "XOR") {
				out = wire_to_val[gate.op1] ^ wire_to_val[gate.op2];
			} else {
				assert(0);
			}
			if (gate.out == "pbm") {
				static int breakhere = 0;
				breakhere++;
			}

			wire_to_val[gate.out] = out;
			if (gate.out[0] == 'z') {
				zbits_written++;
			}
			gates[i] = gates.back();
			gates.pop_back();
		}
	}

	uint64_t final_val = 0;
	for (size_t i = 0; i < zbit_count; i++) {
		string key = "z";
		if (i < 10) {
			key += "0";
		}
		key += to_string(i);

		const uint64_t wire_bit = wire_to_val[key];
		final_val += wire_bit << i;
		cout << key << ": " << wire_bit << endl;
	}
	cout << "Final value is " << final_val << endl;
}

void part_two() {

	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	unordered_map<string, uint8_t> wire_to_val;

	// Collect starting values for wires
	string line;
	getline(file, line);

	while (!line.empty()) {
		const string& wire_name = line.substr(0, 3);
		const uint8_t wire_value = atoi(&line[5]);
		wire_to_val[wire_name] = wire_value;
		getline(file, line);

		//cout << wire_name << ": " << wire_value << endl;
	}

	// Build gates
	vector<Gate> gates;
	uint64_t zbit_count = 0;

	while (getline(file, line)) {
		stringstream ss(line);
		string extra;
		Gate new_gate;
		ss >> new_gate.op1 >> new_gate.operand >> new_gate.op2 >> extra >> new_gate.out;
		gates.push_back(new_gate);

		if (new_gate.out[0] == 'z') {
			const uint64_t z_idx = atoi(new_gate.out.substr(1).c_str()) + (uint64_t)1;
			if (z_idx > zbit_count) {
				zbit_count = z_idx;
			}
		}
	}
	//cout << "z has " << zbit_count << " bits" << endl;

	uint64_t zbits_written = 0;
	while (zbits_written < zbit_count) {
		for (int64_t i = gates.size() - 1; i >= 0; i--) {
			const Gate& gate = gates[i];
			if (!std_contains(wire_to_val, gate.op1) ||
				!std_contains(wire_to_val, gate.op2)) {
				continue;
			}

			assert(!std_contains(wire_to_val, gate.out));

			uint8_t out;
			uint8_t op1 = wire_to_val[gate.op1], op2 = wire_to_val[gate.op2];
			if (gate.operand == "AND") {
				out = wire_to_val[gate.op1] & wire_to_val[gate.op2];
			}
			else if (gate.operand == "OR") {
				out = wire_to_val[gate.op1] | wire_to_val[gate.op2];
			}
			else if (gate.operand == "XOR") {
				out = wire_to_val[gate.op1] ^ wire_to_val[gate.op2];
			}
			else {
				assert(0);
			}
			if (gate.out == "pbm") {
				static int breakhere = 0;
				breakhere++;
			}

			wire_to_val[gate.out] = out;
			if (gate.out[0] == 'z') {
				zbits_written++;
			}

			cout << "[" << gate.op1 << " " << gate.operand << " " << gate.op2 << " -> " << gate.out << "] ";
			gates[i] = gates.back();
			gates.pop_back();
		}
		cout << endl;
	}

	uint64_t final_val = 0;
	for (size_t i = 0; i < zbit_count; i++) {
		string key = "z";
		if (i < 10) {
			key += "0";
		}
		key += to_string(i);

		const uint64_t wire_bit = wire_to_val[key];
		final_val += wire_bit << i;
		cout << key << ": " << wire_bit << endl;
	}
	cout << "Final value is " << final_val << endl;
}

/**
 *
 */
int main() {
//	cout << "Part one..........................................................\n";	
	//part_one();

	cout << "\n\nPart two..........................................................\n";
	part_two();//
	return 1;
}

// 56939028423824