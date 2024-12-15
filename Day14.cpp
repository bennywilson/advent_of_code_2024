#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <sstream>

using namespace std;
using namespace std::chrono;

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

	int64_t x;
	int64_t y;
};

struct Robot {
	Vec2 pos;
	Vec2 vel;
};

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	uint32_t sim_length = 1;
	const uint32_t board_width = 101;
	const uint32_t board_height = 103;
	const uint32_t middle_row = board_height / 2;
	const uint32_t middle_column = board_width / 2;

	string line;
	unordered_map<uint64_t, uint64_t> robot_idx;

	uint64_t tl_quad = 0;
	uint64_t tr_quad = 0;
	uint64_t bl_quad = 0;
	uint64_t br_quad = 0;
	int num_robots = 0;
	vector<Robot> robots;

	while (std::getline(file, line)) {
		stringstream ss(line);
		Vec2 pos;
		Vec2 vel;

		num_robots++;
		char empty_char;
		ss >> empty_char >> empty_char;
		ss >> pos.x >> empty_char >> pos.y;
		ss >> empty_char >> empty_char;
		ss >> vel.x >> empty_char >> vel.y;
		
		Robot new_robot;
		new_robot.pos = pos;
		new_robot.vel = vel;
		robots.push_back(new_robot);
	}

	int longest_span = 0;
	int count = 0;
	while (true) {
		count++;
		robot_idx.clear();
		for (int i = 0; i < robots.size(); i++) {
			const Vec2 pos = robots[i].pos;
			const Vec2 vel = robots[i].vel;

			const Vec2 travel_vec = vel * (sim_length + count);
			const Vec2 new_pos = pos + travel_vec;
			if (new_pos.x < 0) {
				new_pos.x = board_width - (board_width - new_pos.x) % board_width;
			}
			new_pos.x %= board_width;

			if (new_pos.y < 0) {
				new_pos.y = board_height - (board_height - new_pos.y) % board_height;
			}
			new_pos.y %= board_height;

			if (new_pos.x == middle_column || new_pos.y == middle_row) {
				continue;
			}

			if (new_pos.x < middle_column) {
				if (new_pos.y < middle_row) {
					tl_quad++;
				} else {
					bl_quad++;
				}
			} else {
				if (new_pos.y < middle_row) {
					tr_quad++;
				}
				else {
					br_quad++;
				}
			}

			const uint64_t board_idx = new_pos.x + new_pos.y * board_width;
			robot_idx[board_idx]++;
		}

		bool bDrawIt = false;

		for (uint64_t y = 0; y < board_height; y++) {
			int cur_span = 0;
			for (uint64_t x = 0; x < board_width; x++) {
				const uint64_t board_idx = x + y * board_width;
				if (robot_idx[board_idx] > 0) {
					cur_span++;
				} else {
				}
			}
			if (cur_span > 20) {
				longest_span = cur_span;
				for (uint64_t x = 0; x < board_width; x++) {
					int cur_span = 0;
					for (uint64_t y = 0; y < board_height; y++) {
						const uint64_t board_idx = x + y * board_width;
						if (robot_idx[board_idx] > 0) {
							cur_span++;
						}
					}
					if (cur_span > 20) {
						cout << "Christmas tree at " << sim_length + count << " seconds" << endl;
						break;
					}
				}
			}
		}

		if (bDrawIt) {
			for (uint64_t y = 0; y < board_height; y++) {
				bool all_robots = true;
				for (uint64_t x = 0; x < board_width; x++) {
					const uint64_t board_idx = x + y * board_width;
					if (robot_idx[board_idx] > 0) {
							cout << "*";
					}
					else {
						all_robots = false;
							cout << ".";
					}
				}
				if (all_robots) {
					bDrawIt = true;
				}
				cout << endl;
			}
			getchar();
		}
	}
	cout << "Score is " << tl_quad * bl_quad * tr_quad * br_quad << endl;
	cout << "Num robots is " << num_robots << endl;
}

/**
 *
 */
int main() {
	 part_one();
}
