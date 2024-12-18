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
	Key(uint64_t _idx, Vec2 _dir) :
		idx(_idx),
		dir(_dir),
		score(0) {}

	Key(uint64_t _idx, Vec2 _dir, uint64_t _score, unordered_set<uint64_t>& _inPath) :
		idx(_idx),
		dir(_dir),
		score(_score),
		path(_inPath) {}

	uint64_t idx;
	uint64_t score;
	Vec2 dir;
	unordered_set<uint64_t> path;

	bool operator==(const Key& op1) const { return idx == op1.idx && dir == op1.dir; }
};

namespace std {
	template<> struct hash<Key> {
		size_t operator()(const Key& r) const {
			int32_t dir_x = (int32_t) r.dir.x;
			int32_t dir_y = (int32_t)r.dir.y;
			size_t res = hash<uint64_t>{}(r.idx) ^ hash<int64_t>{}(r.dir.x) ^ hash<int64_t>{}(r.dir.y);
			return res;
		}
	};
};

void find_smallest_score(const Vec2& start_pos, Vec2& cur_dir, uint64_t cur_score, uint64_t& smallest_score, unordered_set<uint64_t>& out_set) {
	queue<Key> q;
	unordered_set<uint64_t> visited;
;
	q.push(Key(start_pos.index(), cur_dir));
	unordered_map<Key, uint64_t> cost;

	while (!q.empty()) {
		const Key cur_key = q.front();
		q.pop();

		const uint64_t cur_score = cur_key.score;
		if (cost.find(cur_key) != cost.end() && cost[cur_key] < cur_score) {
			// Been here before at a lower cost
			continue;
		}
		cost[cur_key] = cur_score;

		if (g_board[cur_key.idx] == '#') {
			continue;
		}

		if (g_board[cur_key.idx] == 'E') {
			if (cur_score < smallest_score) {
				smallest_score = cur_score;

				// New smallest path
				out_set = cur_key.path;
			} else if (cur_score == smallest_score) {
				// Combine with other paths matching this smallest size
				out_set.insert(cur_key.path.begin(), cur_key.path.end());
			}
			continue;
		}

		if (visited.find(cur_key.idx) != visited.end()) {
			continue;
		}

		const Vec2 cur_pos(cur_key.idx % g_board_width, cur_key.idx / g_board_width);
		const Vec2 forward_pos = cur_pos + cur_key.dir;
		unordered_set<uint64_t> new_path = cur_key.path;
		new_path.insert(cur_pos.index());
		q.push(Key(forward_pos.index(), cur_key.dir, cur_score + 1, new_path));

		Vec2 perp_dir(cur_key.dir.y, cur_key.dir.x);
		const Vec2 adjacent_1 = cur_pos + perp_dir;
		q.push(Key(adjacent_1.index(), perp_dir, cur_score + 1001, new_path));

		perp_dir = perp_dir *-1;
		Vec2 adjacent_2 = cur_pos + perp_dir;
		q.push(Key(adjacent_2.index(), perp_dir, cur_score + 1001, new_path));
	}
}



/**
 *
 */
int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
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
			const size_t robot_x = line.find_first_of('S');
			if (robot_x != line.npos) {
				robot.x = robot_x;
				robot.y = g_board_height;
			}
		}
		g_board_height++;
	}

	Vec2 cur_dir(1, 0);
	uint64_t smallest_score = INT64_MAX;
	unordered_set<uint64_t> final_set;
	find_smallest_score(robot, cur_dir, 0, smallest_score, final_set);
	cout << "Smallest score is " << smallest_score << " " << "Num covered is " << final_set.size() + 1;

	return 1;
}
