#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>

using namespace std;
using namespace std::chrono;

string g_board;
int64_t g_board_width = 7;
int64_t g_board_height = 7;



void print_board() {

/*	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

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
	SetConsoleMode(hStdOut, originalMode);*/

	string the_output;
	for (int64_t y = 0; y < g_board_height; y++) {
		for (int64_t x = 0; x < g_board_width; x++) {
			the_output += g_board[x + y * g_board_width];
		}
		the_output += "\n";
		//		cout << endl;
	}
	printf("%s", the_output.c_str());
	//	cout << the_output << endl;
	//Sleep(100);
}

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

struct Key {
	Key() { }

	Key(uint64_t _idx, Vec2 _prev = Vec2(-1,-1)) :
		idx(_idx),
		prev(_prev) {}

	uint64_t idx;
	Vec2 prev;

	bool operator==(const Key& op1) const { return idx == op1.idx; }
};

namespace std {
	template<> struct hash<Key> {
		size_t operator()(const Key& r) const {
			size_t res = hash<uint64_t>{}(r.idx);
			return res;
		}
	};
};

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	int64_t g_board_width = 7;
	int64_t g_board_height = 7;
	Vec2 goal_pos(6, 6);

	vector<Vec2> corrupted_bytes;

	string line;
	char empty_char;
	Vec2 new_byte;
	while (file >> new_byte.x) {
		file >> empty_char >> new_byte.y;
		
		cout << "New byte is " << new_byte.x << "," << new_byte.y << endl;
	}

	g_board = "...#.....#..#.....#.....#..#..#..#..#..#..#.#....";
	queue<Key> q;
	q.push(Key(Vec2(0, 0).index()));
	unordered_map<uint64_t, Key> visited;

//	bool path_found = false;
	while (!q.empty()) {
		const Key cur_key = q.front();
		const uint64_t cur_idx = cur_key.idx;
		q.pop();

		const Vec2 cur_pos(cur_idx);
		if (visited.find(cur_idx) != visited.end()) {
			continue;
		}

		if (g_board[cur_idx] == '#') {
			continue;
		}

		visited[cur_idx] = cur_key;
		if (cur_idx == goal_pos.index()) {
			static int breakhere = 0;
			breakhere++;
			break;
		}
		const Vec2 left = cur_pos + Vec2(-1, 0);
		if (left.valid()) {
			q.push(Key(left.index(), cur_pos));
		}
		const Vec2 right = cur_pos + Vec2(1, 0);
		if (right.valid()) {
			q.push(Key(right.index(), cur_pos));
		}
		const Vec2 up = cur_pos + Vec2(0, -1);
		if (up.valid()) {
			q.push(Key(up.index(), cur_pos));
		}
		const Vec2 down = cur_pos + Vec2(0, 1);
		if (down.valid()) {
			q.push(Key(down.index(), cur_pos));
		}

	}

	Vec2 back_track = goal_pos;
	while (back_track.index() != 0) {
		const Key cur_key = visited.find(back_track.index())->second;
		g_board[cur_key.idx] = 'O';
		back_track = cur_key.prev;
		
	}
	
	print_board();
	/*auto it = visited.find(Vec2(5,5).index());
	if (it != visited.end()) {
		uint64_t idx = it->idx;
		while (idx != 0) {
			g_board[idx] = 'O';

			idx = it->prev.index();			
			it = visisted.find
		}
	}*/
}

/**
 *
 */
int main() {
	cout << "Part one...\n";
	part_one();

}

// 6,2,7,2,3,1,6,0,5,