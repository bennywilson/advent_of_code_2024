#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <windows.h>
#include <conio.h>

using namespace std;
using namespace std::chrono;

string g_board;
int64_t g_board_width = 0;
int64_t g_board_height = 0;

/**
 *	Global Functions
 */
struct Vec2 {
	Vec2() :
		x(0),
		y(0) {}

	Vec2(const int64_t in_x, const int64_t in_y) :
		x(in_x),
		y(in_y) {}

	Vec2 operator+(const Vec2& op2) const {
		return Vec2(x + op2.x, y + op2.y);
	}

	Vec2 operator-(const Vec2& op2) const {
		return Vec2(x - op2.x, y - op2.y);
	}

	Vec2 operator*(const int64_t op2) const {
		return Vec2(op2 * x, op2 * y);
	}

	bool operator==(const Vec2& op2) {
		return x == op2.x && y == op2.y;
	}

	bool operator!=(const Vec2& op2) const {
		return x != op2.x || y != op2.y;
	}

	bool valid() const {
		return x >= 0 && x < g_board_width && y >= 0 && y < g_board_height;
	}

	uint64_t index() const {
		return x + y * g_board_width;
	}

	int64_t x;
	int64_t y;
};

void print_board() {
	
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD mode = 0;
	if (!GetConsoleMode(hStdOut, &mode)) {
		return;
	}

	// Hold original mode to restore on exit to be cooperative with other command-line apps.
	const DWORD originalMode = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	// Try to set the mode.
	if (!SetConsoleMode(hStdOut, mode)) {
		return;
	}

	// Write the sequence for clearing the display.
	DWORD written = 0;
	PCWSTR sequence = L"\x1b[2J";
	if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL)) {
		SetConsoleMode(hStdOut, originalMode);
		return;
	}

	sequence = L"\x1b[3J";
	if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL)) {
		SetConsoleMode(hStdOut, originalMode);
		return;

	}
	sequence = L"\x1b[H";
	if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL)) {
		SetConsoleMode(hStdOut, originalMode);
		return;

	}
	SetConsoleMode(hStdOut, originalMode);

	string the_output;
	for (int64_t y = 0; y < g_board_height; y++) {
		for (int64_t x = 0; x < g_board_width; x++) {
			the_output +=  g_board[x + y * g_board_width];
		}
		the_output += "\n";
//		cout << endl;
	}
printf("%s", the_output.c_str());
//	cout << the_output << endl;
	Sleep(100);
}

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	Vec2 robot(-1, -1);
	string line;
	while (std::getline(file, line)) {
		if (line.size() == 0) {
			break;
		}
		g_board += line;
		g_board_width = line.size();

		if (robot.valid() == false) {
			const size_t robot_x = line.find_first_of('@');
			if (robot_x != line.npos) {
				robot.x = robot_x;
				robot.y = g_board_height;
			}
		}
		g_board_height++;
	}

	cout << "\n\nStarting board:\n" << endl;
	print_board();

	vector<char> move_cmds;
	char move_cmd;

	while (file >> move_cmd) {
		move_cmds.push_back(move_cmd);
	}

	// Run sim
	for (auto cmd: move_cmds) {
		Vec2 search_dir(0, 0);
		switch (cmd) {
			case '<': search_dir.x = -1; break;
			case '>': search_dir.x = 1; break;
			case '^': search_dir.y = -1; break;
			case 'v': search_dir.y = 1; break;
		}

		int push_state = -1;
		Vec2 open_spot = robot + search_dir;
		while (open_spot.valid()) {
			char board_char = g_board[open_spot.index()];
			switch (board_char) {
				case '.': push_state = 1; break;
				case '#': push_state = 0; break;
			}
			if (push_state != -1) {
				break;
			}
			open_spot = open_spot + search_dir;
		}

		if (push_state == 0) {
			continue;
		}
		
		Vec2 dest = open_spot;
		while (dest != robot) {
			const uint64_t cur_idx = dest.index();
			const uint64_t prev_idx = (dest - search_dir).index();

			g_board[cur_idx] = g_board[prev_idx];
			g_board[prev_idx] = '.';

			dest = dest - search_dir;
		}
		robot = robot + search_dir;

		//print_board();
		//getchar();
	}

	cout << "\n\nFinal board:\n";
	print_board();

	// Score it!
	uint64_t total_score = 0;
	for (int i = 0; i < g_board.size(); i++) {
		if (g_board[i] != 'O') {
			continue;
		}
		const uint64_t x = i % g_board_width;
		const uint64_t y = i / g_board_width;
		total_score += 100 * y + x;
	}

	cout << "Score is " << total_score << endl;
}

bool push_check(const Vec2& start, const Vec2& dir, const bool perform_pushes) {
	if (dir.x != 0) {
		int push_state = -1;
		Vec2 open_spot = start + dir;
		while (open_spot.valid()) {
			const char board_char = g_board[open_spot.index()];
			switch (board_char) {
				case '.': push_state = 1; break;
				case '#': push_state = 0; break;
			}
			if (push_state != -1) {
				break;
			}
			open_spot = open_spot + dir;
		}

		if (perform_pushes) {
			Vec2 dest = open_spot;
			while (dest != start) {
				const uint64_t cur_idx = dest.index();
				const uint64_t prev_idx = (dest - dir).index();

				g_board[cur_idx] = g_board[prev_idx];
				g_board[prev_idx] = '.';

				dest = dest - dir;
			}
		}
		return push_state == 1;
	}

	const Vec2 next_pos = start + dir;
	if (!next_pos.valid() || g_board[next_pos.index()] == '#') {
		return false;
	}

	const uint64_t next_idx = next_pos.index();
	if (g_board[next_idx] == '.') {
		return true;
	}

	Vec2 left_side;
	Vec2 right_side;
	if (g_board[next_idx] == '[') {
		left_side = next_pos;
		right_side = next_pos + Vec2(1, 0);
	} else if (g_board[next_idx] == ']') {
		right_side = next_pos;
		left_side = next_pos - Vec2(1, 0);
	}

	if (!push_check(left_side, dir, perform_pushes) || !push_check(right_side, dir, perform_pushes)) {
		return false;
	}
	if (perform_pushes) {
		g_board[(left_side + dir).index()] = '[';
		g_board[left_side.index()] = '.';
		g_board[(right_side + dir).index()] = ']';
		g_board[right_side.index()] = '.';
	}
	return true;
}

void part_two() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	Vec2 robot(-1, -1);

	string line;
	while (std::getline(file, line)) {
		if (line.size() == 0) {
			break;
		}

		size_t dst_idx = g_board.size();
		g_board.resize(g_board.size() + line.size() * 2);
		for (const auto it: line) {
			if (it == 'O') {
				g_board[dst_idx++] = '[';
				g_board[dst_idx++] = ']';
			} else if (it == '@') {
				g_board[dst_idx++] = '@';
				g_board[dst_idx++] = '.';
			} else {
				g_board[dst_idx++] = it;
				g_board[dst_idx++] = it;
			}
		}
		g_board_width = line.size() * 2;

		if (robot.valid() == false) {
			const size_t robot_x = line.find_first_of('@') * 2;
			if (robot_x != line.npos) {
				robot.x = robot_x;
				robot.y = g_board_height;
			}
		}
		g_board_height++;
	}

	cout << "\n\nStarting board:\n" << endl;
	print_board();
	getchar();
	print_board();
	Sleep(1500);

	vector<char> move_cmds;
	char move_cmd;

	while (file >> move_cmd) {
		move_cmds.push_back(move_cmd);
	}

	bool first_run = true;
	for (auto cmd : move_cmds) {
		Vec2 search_dir(0, 0);
		switch (cmd) {
			case '<': search_dir.x = -1; break;
			case '>': search_dir.x = 1; break;
			case '^': search_dir.y = -1; break;
			case 'v': search_dir.y = 1; break;
		}

		if (!push_check(robot, search_dir, false)) {
			continue;
		}
		push_check(robot, search_dir, true);

		g_board[robot.index()] = '.';
		robot = robot + search_dir;
		g_board[robot.index()] = '@';

		//Sleep(1500);
		print_board();

		//getchar();
	}

	cout << "\n\nFinal board:\n";
	print_board();

	// Score
	uint64_t total_score = 0;
	for (int i = 0; i < g_board.size(); i++) {
		if (g_board[i] != '[') {
			continue;
		}
		const uint64_t x = i % g_board_width;
		const uint64_t y = i / g_board_width;
		total_score += 100 * y + x;
	}

	cout << "Score is " << total_score << endl;
}

/**
 *
 */
int main() {

	cout << "Part one................." << endl;
//	part_one();

	cout << "Part two................." << endl;
	g_board.clear();
	g_board_width = 0;
	g_board_height = 0;

	part_two();
}
