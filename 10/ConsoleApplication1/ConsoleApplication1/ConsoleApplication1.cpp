#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

int g_board_width = 0;
string g_board;
unordered_set<int> g_visited;

int trail_score(const int x, const int y, const int cur_count) {
	if (x < 0 || x >= g_board_width || y < 0 || y >= g_board_width) {
		return 0;
	}

	const int board_index = x + y * g_board_width;
	const int board_val = g_board[board_index] - '0';


	if (board_val != cur_count + 1) {
		return 0;
	}


	if (cur_count == 8 && board_val == 9) {
		if (g_visited.find(board_index) != g_visited.end()) {
			return 0;
		}

		//g_visited.insert(board_index);
		return 1;
	}

	int score = trail_score(x - 1, y, board_val);
	score += trail_score(x, y + 1, board_val);
	score += trail_score(x + 1, y, board_val);
	score += trail_score(x, y - 1, board_val);
	return score;
}

/**
 *
 */
int main() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string line;
	while (getline(file, line)) {
		g_board_width = (int)line.size();
		g_board += line;
	}

	for (int i = 0; i < g_board.size(); i++) {
		cout << g_board[i];
		if ((i + 1) % g_board_width == 0) {
			cout << endl;
		}
	}

	int total_score = 0;
	for (int i = 0; i < g_board.size(); i++) {
		g_visited.clear();
		const int x = i % g_board_width;
		const int y = i / g_board_width;
		total_score += trail_score(x, y, -1);

	}

	cout << "Total score is " << total_score << endl;
}