﻿#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>
#include <fcntl.h>

#include <io.h>
#include <stdio.h>
#include <windows.h>

using namespace std;
using namespace std::chrono;

bool g_log = false;
int64_t g_board_width = 0;
int64_t g_board_height = 0;
string g_board;

template<typename V, typename T>
bool std_contains(const vector<V>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template<typename V, typename T>
bool std_contains(const V& vec, const T& value) {
	return vec.find(value) != vec.end();
}

struct Vec2 {
	Vec2() :
		x(0),
		y(0) {}

	Vec2(const int64_t in_x, const int64_t in_y) :
		x(in_x),
		y(in_y) {}

	Vec2(const int64_t idx) {
		x = idx % g_board_width;
		y = idx / g_board_width;
	}

	Vec2 operator+(const Vec2& op2) const {
		return Vec2(x + op2.x, y + op2.y);
	}

	Vec2 operator-(const Vec2& op2) const {
		return Vec2(x - op2.x, y - op2.y);
	}

	Vec2 operator*(const int64_t op2) const {
		return Vec2(op2 * x, op2 * y);
	}

	bool operator==(const Vec2& op2) const {
		return x == op2.x && y == op2.y;
	}

	bool operator!=(const Vec2& op2) const {
		return x != op2.x || y != op2.y;
	}

	bool valid() const {
		return x >= 0 && x < g_board_width && y >= 0 && y < g_board_height;
	}

	int64_t index() const {
		if (valid()) {
			return x + y * g_board_width;
		} else {
			return -1;
		}
	}

	int64_t x;
	int64_t y;
};


void print_board(const bool clear_console, const Vec2& start_pos, const Vec2& end_pos, unordered_map<int64_t, int64_t>& distances) {

	//_setmode(_fileno(stdout), _O_U16TEXT);

	if (clear_console) {
		//	SetConsoleOutputCP(1200);
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
	}

	/*Key backtrack_pos = visited[end_pos.index()];
	const uint64_t start_idx = start_pos.index();
	vector<uint64_t> path;

	unordered_set<uint64_t> path_set;

	while (backtrack_pos.idx != start_idx) {
		path_set.insert(backtrack_pos.idx);
		backtrack_pos = visited[backtrack_pos.prev];
	}*/
	//cout << "Num steps = " << path_set.size() << endl << endl;
	int o_count = 0;
	string the_output;
	//cout.precision(2);
	//auto new_str = std::string(n_zero - std::min(n_zero, old_str.length()), '0') + old_str;
	for (int64_t y = 0; y < g_board_height; y++) {
		for (int64_t x = 0; x < g_board_width; x++) {
			const int64_t idx = Vec2(x, y).index();
			if (std_contains(distances, idx)) {
				if (distances[idx] < 10) {
					the_output += '/';
					the_output += std::to_string(distances[idx]);
				}
				else {
					the_output += std::to_string(distances[idx]);
				}
				o_count++;
			}
			else {
				the_output += g_board[idx];
				the_output += g_board[idx];
			}
		}
		the_output += "\n";
	}
	cout << the_output.c_str() << endl;
	//	printf("%s", the_output.c_str());		
}

int64_t find_distances(const Vec2& cur_pos, const Vec2& prev, const Vec2& goal, const int64_t cur_steps, unordered_map<int64_t, int64_t>& distances) {
	if (!cur_pos.valid()) {
		return INT64_MAX;
	}

	if (g_board[cur_pos.index()] == '#') {
		return INT64_MAX;;
	}

	const int64_t cur_idx = cur_pos.index();
	distances[cur_idx] = cur_steps;

	if (cur_pos == goal) {
		return 1;
	}

	const Vec2 dirs[] = {
		Vec2(1, 0),
		Vec2(-1, 0),
		Vec2(0, 1),
		Vec2(0, -1)
	};

	int64_t dist = INT64_MAX;

	for (int i = 0; i < 4; i++) {
		const Vec2 next_pos = cur_pos + dirs[i];
		if (next_pos == prev) {
			continue;
		}

		if (next_pos.valid()) {
			dist = find_distances(next_pos, cur_pos, goal, cur_steps + 1, distances);
			if (dist != INT64_MAX) {
				break;
			}
		}
	}

	return dist + 1;
}

bool find_shorter_routes(int64_t& short_cuts, const Vec2& cur_pos, const Vec2& prev, const Vec2& goal, const int64_t target_distance, const int64_t cheat_size, unordered_map<int64_t, int64_t>& distances) {
	if (!cur_pos.valid()) {
		return false;
	}

	if (g_board[cur_pos.index()] == '#') {
		return false;
	}

	const int64_t cur_idx = cur_pos.index();

	if (cur_pos == goal) {
		return true;
	}

	const int64_t top = cur_pos.y - cheat_size;
	const int64_t bottom = cur_pos.y + cheat_size;
	const int64_t left = cur_pos.x - cheat_size;
	const int64_t right = cur_pos.x + cheat_size;

	Vec2 dest_pos = Vec2(left, top);
	for (dest_pos.y = top; dest_pos.y <= bottom; dest_pos.y++) {
		for (dest_pos.x = left; dest_pos.x <= right; dest_pos.x++) {
			const int64_t dst_idx = dest_pos.index();
			if (!std_contains(distances, dst_idx)) {
				continue;
			}

			const int64_t manhattan_distance = abs(dest_pos.x - cur_pos.x) + abs(dest_pos.y - cur_pos.y);
			if (manhattan_distance > cheat_size) {
				continue;
			}

			const int64_t start_to_cur = distances[cur_pos.index()];
			const int64_t start_to_dest = distances[dest_pos.index()];
			const int64_t distance_saved = start_to_dest - (start_to_cur + manhattan_distance);
			if (distance_saved >= target_distance) {
				short_cuts++;
			}			
		}
	}

	const Vec2 dirs[] = {
		Vec2(1, 0),
		Vec2(-1, 0),
		Vec2(0, 1),
		Vec2(0, -1)
	};

	bool route_found = false;
	for (int i = 0; i < 4; i++) {
		const Vec2 next_pos = cur_pos + dirs[i];
		if (next_pos == prev) {
			continue;
		}

		if (next_pos.valid()) {
			route_found = find_shorter_routes(short_cuts, next_pos, cur_pos, goal, target_distance, cheat_size, distances);
			if (route_found) {
				break;
			}
		}
	}

	return route_found;
}

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	g_board_height = 0;

	Vec2 start(-1, -1);
	Vec2 end(-1, -1);

	string line;
	while (std::getline(file, line) && line.size() > 0) {
		g_board_width = line.length();
		g_board += line;

		if (!start.valid()) {
			const size_t s = line.find('S');
			if (s != line.npos) {
				start = Vec2(s + g_board_height * g_board_width);
			}
		}

		if (!end.valid()) {
			const size_t g = line.find('E');
			if (g != line.npos) {
				end = Vec2(g + g_board_height * g_board_width);
			}
		}

		g_board_height++;
	}

	unordered_map<int64_t, int64_t> distances;
	find_distances(start, Vec2(-1, -1), end, 0, distances);


	int64_t short_cuts = 0;
	find_shorter_routes(short_cuts, start, Vec2(-1, -1), end, 100, 20, distances);

	print_board(false, start, end, distances);
	cout << "Num short cuts = " << short_cuts << endl;
}


/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";
	part_one();
}
