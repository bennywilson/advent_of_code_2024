#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace std::chrono;

struct Vec2 {
	Vec2() :
		x(0),
		y(0) {}

	Vec2(const int64_t in_x, const int64_t in_y) :
		x(in_x),
		y(in_y) {}


	int64_t x;
	int64_t y;
};

struct ClawMachines {
	Vec2 button_a;
	Vec2 button_b;
	Vec2 prize;
};

void part_one() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	vector<ClawMachines> claw_machines;

	cout << "Loading inputs...\n";
	string line;
	while (std::getline(file, line)) {
		ClawMachines next_machine;

		const size_t button_a_start_x = line.find_first_of("+") + 1;
		const string button_a_x = line.substr(button_a_start_x);
		next_machine.button_a.x = std::atoi(button_a_x.c_str());

		const size_t button_a_start_y = line.find_last_of("+") + 1;
		const string button_a_y = line.substr(button_a_start_y);
		next_machine.button_a.y = std::atoi(button_a_y.c_str());

		std::getline(file, line);
		const size_t button_b_start_x = line.find_first_of("+") + 1;
		const string button_b_x = line.substr(button_b_start_x);
		next_machine.button_b.x = std::atoi(button_b_x.c_str());

		const size_t button_b_start_y = line.find_last_of("+") + 1;
		const string button_b_y = line.substr(button_b_start_y);
		next_machine.button_b.y = std::atoi(button_b_y.c_str());

		std::getline(file, line);
		const size_t prize_start_x = line.find_first_of("=") + 1;
		const string prize_x = line.substr(prize_start_x);
		next_machine.prize.x = std::atoi(prize_x.c_str()) + 10000000000000;

		const size_t prize_start_y = line.find_last_of("=") + 1;
		const string prize_y = line.substr(prize_start_y);
		next_machine.prize.y = std::atoi(prize_y.c_str()) + 10000000000000;

		claw_machines.push_back(next_machine);
		cout << "New machine: A=[" << next_machine.button_a.x << " " << next_machine.button_a.y << "], B=[" << next_machine.button_b.x << " " << next_machine.button_b.y << "], P=[" << next_machine.prize.x << " " << next_machine.prize.y << "]\n" << endl;
		std::getline(file, line);
	}

	uint64_t tokens_spent = 0;

	for (auto &it: claw_machines) {
		const Vec2 a = it.button_a;
		const Vec2 b = it.button_b;
		const Vec2 p = it.prize;

		const int64_t num_a_presses = (p.x * b.y - b.x * p.y) / (a.x * b.y - b.x * a.y);
		const int64_t num_b_presses = (p.y - (a.y * num_a_presses)) / b.y;

		cout << "Playing new machine....\n";
		cout << "	Num a presses is " << num_a_presses << ", num b presses is " << num_b_presses << endl;
		int64_t equation_1 = a.x * num_a_presses + b.x * num_b_presses;
		int64_t equation_2 = a.y * num_a_presses + b.y * num_b_presses;
		cout << equation_1 << ", " << equation_2 << endl;
		if (equation_1 == p.x && equation_2 == p.y) {
			cout << "	Tokens spent!" << num_a_presses * 3 + num_b_presses << endl;
			tokens_spent += num_a_presses * 3 + num_b_presses;
		} else {
			cout << "	Unwinnable!" << endl;
			cout << endl;
		}
		
	}

	cout << "Total tokens spent is " << tokens_spent << endl;
}

/**
 *
 */
int main() {
	part_one();
}