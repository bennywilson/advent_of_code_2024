#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

using namespace std;
using namespace std::chrono;

string g_board;
int32_t g_board_width;
unordered_map<uint32_t, uint32_t> g_visited;

// Part 2 Globals
unordered_set<uint32_t> g_top_edges;
unordered_set<uint32_t> g_bottom_edges;
unordered_set<uint32_t> g_left_edges;
unordered_set<uint32_t> g_right_edges;

/**
 *	Global Functions
 */
struct Vec2 {
	Vec2() :
		x(0),
		y(0) {}

	Vec2(const int in_x, const int in_y) :
		x(in_x),
		y(in_y) {}

	Vec2 operator+(const Vec2& op2) const {
		return Vec2(x + op2.x, y + op2.y);
	}

	Vec2 operator-(const Vec2& op2) const {
		return Vec2(x - op2.x, y - op2.y);
	}

	bool operator==(const Vec2& op2) {
		return x == op2.x && y == op2.y;
	}

	bool valid_pos() const {
		return x >= 0 && x < g_board_width && y >= 0 && y < g_board_width;
	}

	int board_index() const {
		return x + y * g_board_width;
	}

	int x;
	int y;
};

void build_region(const Vec2& pos, char cur_char) {
	if (!pos.valid_pos()) {
		return;
	}

	const uint32_t board_idx = pos.board_index();
	if (g_visited.find(board_idx) != g_visited.end()) {
		return;
	}

	if (g_board[board_idx] != cur_char) {
		return;
	}

	g_visited[board_idx] = cur_char;

	build_region(pos + Vec2(1, 0), cur_char);
	build_region(pos + Vec2(-1, 0), cur_char);
	build_region(pos + Vec2(0, 1), cur_char);
	build_region(pos + Vec2(0, -1), cur_char);
}

/**
 *	Part 1 Functions
 */
void calculate_cost(const Vec2& pos, char cur_char, uint32_t& perimeter, uint32_t& area) {
	if (!pos.valid_pos()) {
		perimeter++;
		return;
	}

	const uint32_t board_idx = pos.board_index();
	if (g_board[board_idx] != cur_char) {
		perimeter++;
		return;
	}

	if (g_visited.find(board_idx) != g_visited.end()) {
		return;
	}

	g_visited[board_idx] = cur_char;
	area++;

	calculate_cost(pos + Vec2(1, 0), cur_char, perimeter, area);
	calculate_cost(pos + Vec2(-1, 0), cur_char, perimeter, area);
	calculate_cost(pos + Vec2(0, 1), cur_char, perimeter, area);
	calculate_cost(pos + Vec2(0, -1), cur_char, perimeter, area);
	return;
}

void part_one() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	list<uint64_t> stones;
	while (std::getline(file, line)) {
		g_board += line;
		g_board_width = max(g_board_width, (int32_t)line.length());
	}

	for (size_t i = 0; i < g_board.size(); i++) {
		const int32_t x = (int32_t)(i % g_board_width);
		const int32_t y = (int32_t)(i / g_board_width);
		build_region(Vec2(x, y), g_board[i]);
	}

	g_visited.clear();
	uint32_t total = 0;

	for (size_t i = 0; i < g_board.size(); i++) {
		const int32_t x = (int32_t)(i % g_board_width);
		const int32_t y = (int32_t)(i / g_board_width);
		uint32_t area = 0;
		uint32_t perimeter = 0;

		calculate_cost(Vec2(x, y), g_board[i], perimeter, area);
		total += area * perimeter;
	}

	cout << "Total = " << total << endl;
}

/**
 *	Part 2 Functions
 */
void clear_line(Vec2 current, const Vec2& dir, const Vec2& perp_dir, const char val, unordered_set<uint32_t>& set) {
	while (current.valid_pos()) {
		const int cur_idx = current.board_index();
		if (g_board[cur_idx] != val) {
			return;
		}

		const Vec2 perp_pos = current + perp_dir;
		if (perp_pos.valid_pos()) {
			if (g_board[perp_pos.board_index()] == val) {
				return;
			}
		}

		set.insert(cur_idx);
		current = current + dir;
	}
}

// returns if edge
bool calculate_cost_2(const int32_t x, const int32_t y, char cur_char, uint32_t& area, uint32_t& cost) {
	if (x < 0 || x >= (int32_t)g_board_width || y < 0 || y >= (int32_t)g_board_width) {
		return true;
	}

	const uint32_t board_idx = x + y * g_board_width;
	if (g_board[board_idx] != cur_char) {
		return true;
	}

	if (g_visited.find(board_idx) != g_visited.end()) {
		return false;
	}

	g_visited[board_idx] = cur_char;
	area++;

	if (calculate_cost_2(x + 1, y, cur_char, area, cost)) {
		if (g_right_edges.find(board_idx) == g_right_edges.end()) {
			cost++;
			clear_line(Vec2(x, y), Vec2(0, 1), Vec2(1, 0), cur_char, g_right_edges);
			clear_line(Vec2(x, y) + Vec2(0, -1), Vec2(0, -1), Vec2(1, 0), cur_char, g_right_edges);
		}
	}

	if (calculate_cost_2(x - 1, y, cur_char, area, cost)) {
		if (g_left_edges.find(board_idx) == g_left_edges.end()) {
			cost++;
			clear_line(Vec2(x, y), Vec2(0, 1), Vec2(-1, 0), cur_char, g_left_edges);
			clear_line(Vec2(x, y) + Vec2(0, -1), Vec2(0, -1), Vec2(-1, 0), cur_char, g_left_edges);
		}
	}

	if (calculate_cost_2(x, y + 1, cur_char, area, cost)) {
		if (g_bottom_edges.find(board_idx) == g_bottom_edges.end()) {
			cost++;
			clear_line(Vec2(x, y), Vec2(1, 0), Vec2(0, 1), cur_char, g_bottom_edges);
			clear_line(Vec2(x, y) + Vec2(-1, 0), Vec2(-1, 0), Vec2(0, 1), cur_char, g_bottom_edges);
		}
	}

	if (calculate_cost_2(x, y - 1, cur_char, area, cost)) {
		if (g_top_edges.find(board_idx) == g_top_edges.end()) {
			cost++;
			clear_line(Vec2(x, y), Vec2(1, 0), Vec2(0, -1), cur_char, g_top_edges);
			clear_line(Vec2(x, y) + Vec2(-1, 0), Vec2(-1, 0), Vec2(0, -1), cur_char, g_top_edges);
		}
	}

	return false;
}

void part_two() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	list<uint64_t> stones;
	while (std::getline(file, line)) {
		g_board += line;
		g_board_width = max(g_board_width, (int32_t)line.length());
		//cout << line << endl;
	}

	for (size_t i = 0; i < g_board.size(); i++) {
		const int32_t x = (int32_t)(i % g_board_width);
		const int32_t y = (int32_t)(i / g_board_width);
		build_region(Vec2(x, y), g_board[i]);
	}

	g_visited.clear();
	uint32_t total = 0;

	for (size_t i = 0; i < g_board.size(); i++) {
		g_top_edges.clear();
		g_bottom_edges.clear();
		g_left_edges.clear();
		g_right_edges.clear();

		const int32_t x = (int32_t)(i % g_board_width);
		const int32_t y = (int32_t)(i / g_board_width);
		uint32_t cost = 0;
		uint32_t area = 0;
		calculate_cost_2(x, y, g_board[i], area, cost);
		// cout << cost << "*" << area << endl;
		total += cost * area;
	}

	cout << "Total = " << total << endl;
}

/**
 *
 */
int main() {
	part_one();
	part_two();
}
