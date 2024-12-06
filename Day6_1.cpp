#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

using namespace std;

/**
 *
 */
struct Vec2 {
	Vec2(const int in_x, const int in_y) :
		x(in_x),
		y(in_y) {}

	Vec2 operator+(const Vec2& op2) const {
		return Vec2(x + op2.x, y + op2.y);
	}

	int board_index(const int board_width) const {
		return x + y * board_width;
	}

	int x;
	int y;
};

/**
 *
 */
int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string board;
	string line;
	int board_width = 0;
	Vec2 guard_pos(-1, -1);
	while (std::getline(file, line)) {
		board += line;

		if (guard_pos.x < 0) {
			guard_pos.y++;
			const size_t guard_x = line.find("^");
			if (guard_x != line.npos) {
				board_width = line.size();
				guard_pos.x = (int)guard_x;
				cout << "Guard found at " << guard_pos.x << "," << guard_pos.y << endl;
			}
		}
	}

	cout << "Board: " << board_width << endl;

	unordered_set<int> visited_indices;

	int guard_dir_index = 0;
	const Vec2 move_dir[] = {
		Vec2(0, -1),
		Vec2(1, 0),
		Vec2(0, 1),
		Vec2(-1, 0),
	};

	int num_visited = 0;
	visited_indices.insert(guard_pos.board_index(board_width));

	while (guard_pos.x >= 0 && guard_pos.x < board_width && guard_pos.y >= 0 && guard_pos.y < board_width) {
		const Vec2 next_pos = guard_pos + move_dir[guard_dir_index];
		const size_t next_index = next_pos.board_index(board_width);
		//cout << "new pos is " << next_pos.x << "," << next_pos.y << endl;
		if (board[next_index] == '#') {
			guard_dir_index = (guard_dir_index + 1) % 4;
		}
		else {
			guard_pos = next_pos;
			if (visited_indices.find(next_index) == visited_indices.end()) {
				num_visited++;
				visited_indices.insert(next_index);
			}
		}
	}
	cout << "Num visited = " << num_visited << endl;
	cout << "Num not visited = " << board.size() - num_visited << endl;
	//    cout << board << endl;
	return 0;
}
