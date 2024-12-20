#include <chrono>
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

template<typename V,typename T>
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

	Vec2(const uint64_t idx) {
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

	uint64_t index() const {
		return x + y * g_board_width;
	}

	int64_t x;
	int64_t y;
};


struct Key {
	Key() : idx(UINT64_MAX),prev(UINT64_MAX), cost(UINT64_MAX) {}

	Key(uint64_t _idx, const uint64_t _prev, const uint64_t _cost) :
		idx(_idx),
		prev(_prev),
		cost(_cost) {}

	uint64_t idx;
	uint64_t prev;
	uint64_t cost;

	bool operator==(const Key& op1) const { return idx == op1.idx; /*&& cheats == op1.cheats;*/ }
};

namespace std {
	template<> struct hash<Key> {
		size_t operator()(const Key& r) const {
			size_t res = hash<uint64_t>{}(r.idx);
			return res;
		}
	};
};


void print_board(const bool clear_console, const Vec2& start_pos, const Vec2& end_pos, unordered_set<uint64_t>& visited) {

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
	for (int64_t y = 0; y < g_board_height; y++) {
		for (int64_t x = 0; x < g_board_width; x++) {
			const uint64_t idx = Vec2(x, y).index();
			if (std_contains(visited, idx)) {
				the_output += "O";
				o_count++;
			}
			else {
				the_output += g_board[idx];
			}
		}
		the_output += "\n";
	}
	cout << the_output.c_str() << endl;
	//	printf("%s", the_output.c_str());		
}

unordered_map<Key, uint64_t> visited;
uint64_t find_path(const Vec2& cur_pos, const Vec2& prev, const Vec2& goal, uint64_t cur_cost, unordered_set<uint64_t>& cheat_indices, vector<uint64_t>& cur_path, vector<vector<uint64_t>>& all_paths) {
	if (!cur_pos.valid()) {
		return 0;
	}

	if (cur_pos == goal) {
		all_paths.push_back(cur_path);
		return 1;
	}

	Key cur_key(cur_pos.index(), prev.index(), cur_cost);
	uint64_t cur_idx = cur_pos.index();
	if (std_contains(visited, cur_key)) {
		return 0;
	}

	if (g_board[cur_pos.index()] == '#') {
		if (cheat_indices.size() == 1 || std_contains(cheat_indices, cur_idx)) {
			return 0;
		}
		cheat_indices.insert(cur_idx);
	}

	const Vec2 dirs[] = {
		Vec2(1, 0),
		Vec2(-1, 0),
		Vec2(0, 1),
		Vec2(0, -1)
	};

	uint64_t path_count = 0;
	if (std_contains(cur_path, cur_pos.index())) {
		static int how = 0;
		how++;
	}
	cur_path.push_back(cur_pos.index());

	visited[cur_key] = cur_cost;
	for (int i = 0; i < 4; i++) {
		const Vec2 next_pos = cur_pos + dirs[i];
		if (next_pos == prev) {
			continue;
		}

		path_count += find_path(next_pos, cur_pos, goal, cur_cost + 1, cheat_indices, cur_path, all_paths);
	}
	cur_path.pop_back();
	cheat_indices.erase(cur_pos.index());
	visited.erase(cur_key);

	if (path_count > 0) {
		return path_count + 1;
	}
	return 0;	
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

	unordered_set<uint64_t> cheat_indices;

	vector<uint64_t> cur_path;
	vector<vector<uint64_t>> all_paths;

	find_path(start, Vec2(-1, -1), end, 0, cheat_indices, cur_path, all_paths);

	uint64_t min_val = UINT64_MAX;
	uint64_t min_idx = INT_MAX;
	uint64_t max_val = 0;

	unordered_map<uint64_t, uint64_t> path_info;

	for (int i = 0; i < all_paths.size(); i++) {
		uint64_t path_len = all_paths[i].size();
		if (path_len < min_val) {
			min_val = all_paths[i].size();
			min_idx = i;
		}
		if (path_len > max_val) {
			max_val = path_len;
		}
	}

	uint64_t num_over_hundred = 0;
	for (int i = 0; i < all_paths.size(); i++) {
		uint64_t savings = (max_val - all_paths[i].size());
		path_info[savings]++;
		if (savings >= 100) {
			num_over_hundred++;
		}
	}

/*
	unordered_set<uint64_t> touched_cells;
	print_board(false, start, end, touched_cells);
	for (int i = 0; i < all_paths.size(); i++) {
		vector<uint64_t>& cur_path = all_paths[i];
		for (int k = 0; k < cur_path.size(); k++) {
			if (std::find(cur_path.begin() + k + 1, cur_path.end(), cur_path[k]) != cur_path.end()) {
				static int what = 0;
				what++;
			}
		}

		if (cur_path.size() == max_val - 64) {

			touched_cells.clear();
			for (size_t j = 0; j < cur_path.size(); j++) {
				touched_cells.insert(cur_path[j]);
				
			}
			print_board(false, start, end, touched_cells);
		}

	}
*/

	for (const auto &it: path_info) {
		cout << "There are " << it.second << " cheats that save " << max_val - it.first << " picoseconds." << endl;
	}
	cout << "There are " << num_over_hundred << " cheats that save at least 100 picoseconds." << endl;
}
/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";
	part_one();
}

// 1441