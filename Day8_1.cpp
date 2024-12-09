#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace std::chrono;

int g_board_width = 0;

struct Vec2 {
	Vec2() :
		x(0),
		y(0) {}

	Vec2(const int in_x, const int in_y) :
		x(in_x),
		y(in_y) {}

	Vec2 operator+(const Vec2& op2) const  {
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


/**
 *
 */
int main() {
	const auto start = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string board;
	string line;

	while (std::getline(file, line)) {
		g_board_width = (int)line.size();
		board += line;
	}

	unordered_set<int> antinodes;
	map<int, vector<Vec2>> antenna;

	Vec2 cur_node(0, 0);
	int idx = 0;
	for (int y = 0; y < g_board_width; y++) {
		for (int x = 0; x < g_board_width; x++, idx++) {
			const int cur_value = board[idx];
			if (cur_value == '.') {
				continue;
			}
			antenna[cur_value].push_back(Vec2(x, y));
		}
	}

	for (auto node_list_it = antenna.begin(); node_list_it != antenna.end(); ++node_list_it) {
		const vector<Vec2>& node_list = node_list_it->second;
		for (int cur = 0; cur < node_list.size(); cur++) {
			const Vec2 cur_pos = node_list[cur];
			for (int check = cur; check < node_list.size(); check++) {
				const Vec2 check_pos = node_list[check];
				const Vec2 cur_to_check = check_pos - cur_pos;

				const Vec2 node_1 = cur_pos - cur_to_check;
				if (node_1.valid_pos()) {
					antinodes.insert(node_1.board_index());
				}

				const Vec2 node_2 = check_pos + cur_to_check;
				if (node_2.valid_pos()) {
					antinodes.insert(node_2.board_index());
				}
			}
		}
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start);
	int manual_count = 0;

	for (int i = 0; i < board.length(); i++) {
		if (i > 1 && i % g_board_width == 0) {
			cout << endl;
		}
		if (std::find(antinodes.begin(), antinodes.end(), i) != antinodes.end()) {
			cout << "#";
			manual_count++;
		} else {
			cout << board[i];
		}
	}

	cout << endl;
	cout << "Num antinodes is " << antinodes.size() << endl << endl;
	cout << "Duration = " << 1e+6 / duration.count() << " ms" << endl;
}

// 301