#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>

using namespace std;
using namespace std::chrono;

bool g_log = false;


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

	Vec2 operator+(const Vec2& op2) const {
		return Vec2(x + op2.x, y + op2.y);
	}

	Vec2 operator-(const Vec2& op2) const {
		return Vec2(x - op2.x, y - op2.y);
	}

	Vec2 operator*(const int64_t op2) const {
		return Vec2(op2 * x, op2 * y);
	}

	Vec2 left() const { return *this - Vec2(1, 0); }
	Vec2 right() const { return *this + Vec2(1, 0); }
	Vec2 up() const { return *this - Vec2(0, 1); }
	Vec2 down() const { return *this + Vec2(0, 1); }

	bool operator==(const Vec2& op2) const {
		return x == op2.x && y == op2.y;
	}

	bool operator!=(const Vec2& op2) const {
		return x != op2.x || y != op2.y;
	}

	bool valid_numpad_pos() const {
		if (x < 0 || x > 2 || y < 0 || y > 3 || (x == 0 && y ==3)) {
			return false;
		}
		return true;
	}

	bool valid_arrowpad_pos() const {
		if (x < 0 || x > 2 || y < 0 || y > 1 || (x == 0 && y == 0)) {
			return false;
		}
		return true;
	}

	int64_t x;
	int64_t y;
};

typedef string sPath;
typedef vector<string> sPathLists;
typedef vector<sPathLists> sSequencePath;

struct PathNode {
	PathNode() : pos(-1, -1) {}
	PathNode(const Vec2& _pos, const Vec2& prev_pos, const string& _path = "") :
		pos(_pos),
		path(_path) {}

	Vec2 pos;
	string path;
};

Vec2 get_numpad_pos(const char num) {
	switch (num) {
		case '7': return Vec2(0, 0);
		case '8': return Vec2(1, 0);
		case '9': return Vec2(2, 0);
		case '4': return Vec2(0, 1);
		case '5': return Vec2(1, 1);
		case '6': return Vec2(2, 1);
		case '1': return Vec2(0, 2);
		case '2': return Vec2(1, 2);
		case '3': return Vec2(2, 2);
		case '0': return Vec2(1, 3);
		case 'A': return Vec2(2, 3);
	}

	assert(0);
	return Vec2(-1, -1);
}

void find_numpad_paths(const char start_digit, const char end_digit, sPathLists& s_paths) {
	const int64_t numpad_width = 3;
	const int64_t numpad_height = 4;
	const Vec2 invalid_numpad_pos(0, 3);

	const Vec2 start_pos = get_numpad_pos(start_digit);
	const Vec2 end_pos = get_numpad_pos(end_digit);

	queue<PathNode> q;
	q.push(PathNode(start_pos, Vec2(-1, -1)));

	while (!q.empty()) {
		const PathNode cur_node = q.front();
		q.pop();
		if (cur_node.pos == end_pos) {
			s_paths.push_back(cur_node.path);
			continue;
		}

		const Vec2& cur_pos = cur_node.pos;
		if (!cur_pos.valid_numpad_pos()) {
			continue;
		}

		if (cur_pos.x < end_pos.x && cur_pos.right().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.right(), cur_pos, cur_node.path + ">"));
		}
		if (cur_pos.x > end_pos.x && cur_pos.left().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.left(), cur_pos, cur_node.path + "<"));
		}
		if (cur_pos.y < end_pos.y && cur_pos.down().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.down(), cur_pos, cur_node.path + "v"));
		}
		if (cur_pos.y > end_pos.y && cur_pos.up().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.up(), cur_pos, cur_node.path + "^"));
		}
	}
}

Vec2 get_arrow_pos(const char num) {
	switch (num) {
		case '^': return Vec2(1, 0);
		case 'A': return Vec2(2, 0);
		case '<': return Vec2(0, 1);
		case 'v': return Vec2(1, 1);
		case '>': return Vec2(2, 1);
	}

	assert(0);
	return Vec2(-1, -1);
}

void find_arrowpad_paths(const char start_digit, const char end_digit, vector<string>& sequences) {
	const int64_t arrowpad_width = 3;
	const int64_t arrowpad_height = 2;
	const Vec2 invalid_arrowpad_pos(0, 0);

	const Vec2 start_pos = get_arrow_pos(start_digit);
	const Vec2 end_pos = get_arrow_pos(end_digit);

	queue<PathNode> q;
	q.push(PathNode(start_pos, Vec2(-1, -1)));

	while (!q.empty()) {
		const PathNode cur_node = q.front();
		q.pop();
		if (cur_node.pos == end_pos) {
			sequences.push_back(cur_node.path);
			continue;
		}

		const Vec2& cur_pos = cur_node.pos;
		if (!cur_pos.valid_arrowpad_pos()) {
			continue;
		}

		if (cur_pos.x < end_pos.x && cur_pos.right().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.right(), cur_pos, cur_node.path + '>'));
		}
		if (cur_pos.x > end_pos.x && cur_pos.left().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.left(), cur_pos, cur_node.path + '<'));
		}
		if (cur_pos.y < end_pos.y && cur_pos.down().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.down(), cur_pos, cur_node.path + 'v'));
		}
		if (cur_pos.y > end_pos.y && cur_pos.up().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.up(), cur_pos, cur_node.path + '^'));
		}
	}
}

/*
x ^ A | x ^ A
< v > | < v >
*/
void finalize_arrowpad(const sSequencePath& src_seq, sPathLists& out_paths) {
	sPathLists path_1 = {src_seq[0]};
	sPathLists path_2;
	for (int path_idx = 1; path_idx < src_seq.size(); path_idx++) {
		vector<string>& src = (path_1.size() > 0) ? (path_1) : (path_2);
		vector<string>& dst = (path_1.size() > 0) ? (path_2) : (path_1);

		for (auto& front: src) {
			for (auto& back: src_seq[path_idx]) {
				const string final = front + "A" + back;
				dst.push_back(final);
			}
		}
		src.clear();
	}
	vector<string>& src2 = (path_1.size() > 0) ? (path_1) : (path_2);
	for (int i = 0; i < src2.size(); i++) {
		out_paths.push_back(src2[i] + 'A');
	}
}

void part_one() {
	const string inputs[] = {
		"803A",
		"528A",
		"586A",
		"341A",
		"319A"
	};
	int64_t complexity_sum = 0;
	for (int input_idx = 0; input_idx < 5; input_idx++) {
		const string& input = inputs[input_idx];

		sPathLists paths1, paths2;
		{
			sPathLists new_paths;
			find_numpad_paths('A', input[0], new_paths);
			vector<sPathLists> s_all_path_lists = { new_paths };

			for (size_t i = 0; i < input.size() - 1; i++) {
				sPathLists new_paths;
				find_numpad_paths(input[i], input[i + 1], new_paths);
				s_all_path_lists.push_back(new_paths);
			}
			finalize_arrowpad(s_all_path_lists, paths1);
		}

		for (int i = 0; i < 2; i++) {
			sPathLists& src_pathlists = (paths1.size() > 0)?(paths1):(paths2);
			sPathLists& dst_pathlists = (paths1.size() > 0) ? (paths2) : (paths1);

			for (size_t i = 0; i < src_pathlists.size(); i++) {
				const string& cur_sequence = src_pathlists[i];

				vector<string> sequences = {};
				find_arrowpad_paths('A', cur_sequence[0], sequences);
				sSequencePath sSequences = { sequences };

				for (size_t i = 0; i < cur_sequence.size() - 1; i++) { // Skip \0 at end
					vector<string> sequences = {};
					find_arrowpad_paths(cur_sequence[i], cur_sequence[i + 1], sequences);
					sSequences.push_back(sequences);
				}

				finalize_arrowpad(sSequences, dst_pathlists);
			}
			src_pathlists.clear();
		}

		size_t min_val = SIZE_MAX;
		vector<string>& src = (paths1.size() > 0) ? (paths1) : (paths2);
		for (size_t l = 0; l < src.size(); l++) {
			if (src[l].size() < min_val) {
				min_val = src[l].size();
			}
		}
		const int64_t input_as_number = std::atoi(input.substr(0, 3).c_str());
		cout << "Final size is " << min_val << endl;
		cout << "Numerical portion of input is " << input_as_number << endl;
		cout << "Score is " << min_val * input_as_number << endl;
		complexity_sum += min_val * input_as_number;
	}
	cout << "Complexity sum is " << complexity_sum << endl;
}

/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";
	part_one();
}
