#include <assert.h>
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

	int64_t x;
	int64_t y;
};

typedef vector<Vec2> Path;
typedef vector<Path> PathsToDigit;

struct PathNode {
	PathNode() : pos(-1, -1) {}
	PathNode(const Vec2& _pos, const Vec2& prev_pos, const vector<Vec2>& prev_path = vector<Vec2>()) :
		pos(_pos),
		path_taken(prev_path) {
			path_taken.push_back(pos);
		}

	Vec2	pos;
	Path path_taken;
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

void find_numpad_paths(const char start_digit, const char end_digit, PathsToDigit& all_paths_to_digits) {
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
			all_paths_to_digits.push_back(cur_node.path_taken);
			continue;
		}

		const Vec2& cur_pos = cur_node.pos;
		if (!cur_pos.valid_numpad_pos()) {
			continue;
		}

		if (cur_pos.x < end_pos.x && cur_pos.right().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.right(), cur_pos, cur_node.path_taken));
		}
		if (cur_pos.x > end_pos.x && cur_pos.left().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.left(), cur_pos, cur_node.path_taken));
		}
		if (cur_pos.y < end_pos.y && cur_pos.down().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.down(), cur_pos, cur_node.path_taken));
		}
		if (cur_pos.y > end_pos.y && cur_pos.up().valid_numpad_pos()) {
			q.push(PathNode(cur_pos.up(), cur_pos, cur_node.path_taken));
		}
	}
}

void create_complete_paths(const PathsToDigit& button1, const PathsToDigit& button2, const PathsToDigit& button3) {
	
	PathsToDigit out;
	for (int i = 0; i < button1.size(); i++) {
		const Path& button1_path = button1[i];
		for (int j = 0; j < button2.size(); j++) {
			const Path& button2_path = button2[j];
			Path button1_to_button2_path = button1_path;
			button1_to_button2_path.insert(button1_to_button2_path.end(), button2_path.begin(), button2_path.end());
			button1_to_button2_path[button1_path.size()] = Vec2(-1, -1);	// Indicates button press
			out.push_back(button1_to_button2_path);
		}
	}

	PathsToDigit out_2;
	for (int i = 0; i < out.size(); i++) {
		const Path& button1_2_path = out[i];
		for (int j = 0; j < button3.size(); j++) {
			const Path& button3_path = button3[j];
			Path button1_to_button3_path = button1_2_path;
			button1_to_button3_path.insert(button1_to_button3_path.end(), button3_path.begin(), button3_path.end());
			button1_to_button3_path[button1_2_path.size()] = Vec2(-1, -1);	// Indicates button press
			out_2.push_back(button1_to_button3_path);
		}
	}

	vector<string> sequences;
	for (int i = 0; i < out_2.size(); i++) {
		Path& cur_path = out_2[i];
		string new_sequence;
		for (int j = 1; j < cur_path.size(); j++) {
			const Vec2& cur_pos = cur_path[j];
			const Vec2& prev_pos = cur_path[(size_t)j - 1];
			if (cur_pos.x < 0) {
				new_sequence.push_back('A');
			} else if (cur_pos.x > prev_pos.x) {
				new_sequence.push_back('<');
			} else if (cur_pos.x > prev_pos.x) {
				new_sequence.push_back('>');
			} else if (cur_pos.y > prev_pos.y) {
				new_sequence.push_back('v');
			} else {
				new_sequence.push_back('^');
			}
		}

		sequences.push_back(new_sequence);
	}

	static int breakhere = 0;
	breakhere++;
}

/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";
	

	const string input = "029A";

	PathsToDigit zero_to_two;
	find_numpad_paths('0', '2', zero_to_two);

	PathsToDigit two_to_nine;
	find_numpad_paths('2', '9', two_to_nine);

	PathsToDigit nine_to_a;
	find_numpad_paths('9', 'A', nine_to_a);

	const vector<vector<Vec2>> paths[] = {
		zero_to_two,
		two_to_nine,
		nine_to_a
	};

	vector<vector<Vec2>> complete_paths;
	create_complete_paths(zero_to_two, two_to_nine, nine_to_a);

	/*cout << "Paths between 'A' and '8':" << endl;
	for (const auto& path: all_paths) {
		for (const auto& node: path) {
			cout << "[" << node.x << ", " << node.y << "] ";
		}
		cout << endl;
	}*/
}
