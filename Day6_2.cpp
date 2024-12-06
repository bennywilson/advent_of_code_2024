
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

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

	Vec2 operator-(const Vec2& op2) const {
		return Vec2(x - op2.x, y - op2.y);
	}

	bool operator==(const Vec2& op2) const {
		return x == op2.x && y == op2.y;
	}

	int board_index(const int board_width) const {
		return x + y * board_width;
	}

	bool valid_pos(const int board_width) const {
		return x >= 0 && x < board_width && y >= 0 && y < board_width;
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
				board_width = (int)line.size();
				guard_pos.x = (int)guard_x;
			}
		}
	}
	const Vec2 starting_pos = guard_pos;

	cout << "Board\n " << board << endl;

	int guard_dir_idx = 0;
	const Vec2 move_dir[] = {
		Vec2(0, -1),
		Vec2(1, 0),
		Vec2(0, 1),
		Vec2(-1, 0),
	};


	int num_infinites = 0;

	Vec2 blocker(0, 0);
	for (blocker.y = 0; blocker.y < board_width; blocker.y++) {
		for (blocker.x = 0; blocker.x < board_width; blocker.x++) {
			const int blocker_idx = blocker.board_index(board_width);
			if (board[blocker_idx] == '#' || board[blocker_idx] == '^' ) {
				continue;
			}
			board[blocker_idx] = '#';

			guard_pos = starting_pos;
			guard_dir_idx = 0;

			set<uint32_t> loop_set;
			while (guard_pos.valid_pos(board_width)) {
				const Vec2 next_pos = guard_pos + move_dir[guard_dir_idx];
				if (next_pos.valid_pos(board_width) == false) {
					break;
				}
				const size_t next_index = next_pos.board_index(board_width);

				if (board[next_index] == '#') {
					guard_dir_idx = (guard_dir_idx + 1) % 4;
				}
				else {
					guard_pos = next_pos;
					const uint32_t check_idx = guard_pos.board_index(board_width);
					const uint32_t look_up = (guard_dir_idx << 24) | check_idx;
					if (loop_set.find(look_up) != loop_set.end()) {
						num_infinites++;
						break;
					}
					loop_set.insert(look_up);
				}
			}
			board[blocker_idx] = '.';
		}
	}

	cout << "Inf loops = " << num_infinites << endl;
	return 0;
}