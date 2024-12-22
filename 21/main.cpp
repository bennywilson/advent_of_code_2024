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

	bool valid_arrowpad_pos() const {
		if (x < 0 || x > 2 || y < 0 || y > 1 || (x == 0 && y == 0)) {
			return false;
		}
		return true;
	}

	int64_t x;
	int64_t y;
};

typedef vector<Vec2> Path;
typedef vector<Path> PathLists;

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

void find_numpad_paths(const char start_digit, const char end_digit, PathLists& all_paths_to_digits) {
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

void finalize_numpad(vector<PathLists>& path_lists, vector<string>& sequences) {
	
	//for (auto path_list = path_lists.begin(); path_list != path_lists.end() - 1; ++path_list) {
	PathLists path_list_1 = path_lists[0];
	PathLists path_list_2;

	for (int pathlist_idx = 1; pathlist_idx < path_lists.size(); pathlist_idx++) {
		PathLists& src = (path_list_1.size() > 0)?(path_list_1):(path_list_2);
		PathLists& dst = (path_list_1.size() > 0) ? (path_list_2) : (path_list_1);

		for (auto& front_path: src) {
			for (auto& back_path: path_lists[pathlist_idx]) {
				Path new_path = front_path;
				new_path.insert(new_path.end(), back_path.begin(), back_path.end());
				dst.push_back(new_path);
			}
		}
		src.clear();
	}

	PathLists& src = (path_list_1.size() > 0) ? (path_list_1) : (path_list_2);
	for (int i = 0; i < src.size(); i++) {
		Path& cur_path = src[i];
		string new_sequence;
		for (int j = 1; j < cur_path.size(); j++) {
			const Vec2& cur_pos = cur_path[j];
			const Vec2& prev_pos = cur_path[(size_t)j - 1];
			if (cur_pos == prev_pos) {
				new_sequence.push_back('A');
			}
			else if (cur_pos.x < prev_pos.x) {
				new_sequence.push_back('<');
			}
			else if (cur_pos.x > prev_pos.x) {
				new_sequence.push_back('>');
			}
			else if (cur_pos.y > prev_pos.y) {
				new_sequence.push_back('v');
			}
			else {
				new_sequence.push_back('^');
			}
		}
		new_sequence.push_back('A');
cout << new_sequence << endl;
		sequences.push_back(new_sequence);
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

void find_arrowpad_paths(const char start_digit, const char end_digit, PathLists& all_paths_to_digits) {
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
			all_paths_to_digits.push_back(cur_node.path_taken);
			continue;
		}

		const Vec2& cur_pos = cur_node.pos;
		if (!cur_pos.valid_arrowpad_pos()) {
			continue;
		}

		if (cur_pos.x < end_pos.x && cur_pos.right().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.right(), cur_pos, cur_node.path_taken));
		}
		if (cur_pos.x > end_pos.x && cur_pos.left().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.left(), cur_pos, cur_node.path_taken));
		}
		if (cur_pos.y < end_pos.y && cur_pos.down().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.down(), cur_pos, cur_node.path_taken));
		}
		if (cur_pos.y > end_pos.y && cur_pos.up().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.up(), cur_pos, cur_node.path_taken));
		}
	}
}

void finalize_arrowpad(vector<PathLists>& path_lists, vector<string>& sequences) {
	PathLists path_list_1 = path_lists[0];
	PathLists path_list_2;

	for (int pathlist_idx = 1; pathlist_idx < path_lists.size(); pathlist_idx++) {
		PathLists& src = (path_list_1.size() > 0) ? (path_list_1) : (path_list_2);
		PathLists& dst = (path_list_1.size() > 0) ? (path_list_2) : (path_list_1);

		for (auto& front_path : src) {
			for (auto& back_path : path_lists[pathlist_idx]) {
				Path new_path = front_path;
				new_path.insert(new_path.end(), back_path.begin(), back_path.end());
				dst.push_back(new_path);
			}
		}
		src.clear();
	}

	PathLists& src = (path_list_1.size() > 0) ? (path_list_1) : (path_list_2);
	for (int i = 0; i < src.size(); i++) {
		Path& cur_path = src[i];
		string new_sequence;
		for (int j = 1; j < cur_path.size(); j++) {
			const Vec2& cur_pos = cur_path[j];
			const Vec2& prev_pos = cur_path[(size_t)j - 1];
			if (cur_pos == prev_pos) {
				new_sequence.push_back('A');
			}
			else if (cur_pos.x < prev_pos.x) {
				new_sequence.push_back('<');
			}
			else if (cur_pos.x > prev_pos.x) {
				new_sequence.push_back('>');
			}
			else if (cur_pos.y > prev_pos.y) {
				new_sequence.push_back('v');
			}
			else {
				new_sequence.push_back('^');
			}
		}
		new_sequence.push_back('A');
		sequences.push_back(new_sequence);
	}
}

void process_arrowpad(const vector<string>& in, vector<string>& out, const bool trim) {

	for (size_t i = 0; i < in.size(); i++) {
		const string& cur_sequence = in[i];
	
		PathLists arrowpad_pathlists;
		find_arrowpad_paths('A', cur_sequence[0], arrowpad_pathlists);
		vector<PathLists> path_lists = { arrowpad_pathlists };
	
		for (size_t i = 0; i < cur_sequence.size() - 1; i++) { // Skip \0 at end
			PathLists arrowpad_pathlists;
			find_arrowpad_paths(cur_sequence[i], cur_sequence[i + 1], arrowpad_pathlists);
			path_lists.push_back(arrowpad_pathlists);
		}
	
		finalize_arrowpad(path_lists, out);
	}
	
	if (trim) {
		// Only keep the shortest paths
		size_t min_len = SIZE_MAX;
		for (size_t i = 0; i < out.size(); i++) {
			if (out[i].size() < min_len) {
				min_len = out[i].size();
			}
		}
		auto it = out.begin();
		while (it != out.end()) {
	
			if (it->size() > min_len) {
				it = out.erase(it);
			}
			else {
				++it;
			}
		}
	}	
}

void part_one() {
	/*	const string inputs[] = {
			"029A",
			"980A",
			"179A",
			"456A",
			"379A"
		};*/
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

		vector<string> robot_1_output;
		{
			PathLists path_lists;
			find_numpad_paths('A', input[0], path_lists);
			vector<PathLists> all_path_lists = { path_lists };

			for (size_t i = 0; i < input.size() - 1; i++) {
				PathLists path_lists;
				find_numpad_paths(input[i], input[i + 1], path_lists);
				all_path_lists.push_back(path_lists);
			}
			finalize_numpad(all_path_lists, robot_1_output);
		}

		vector<string> robot_2_output;
		process_arrowpad(robot_1_output, robot_2_output, true);

		vector<string> robot_3_output;
		process_arrowpad(robot_2_output, robot_3_output, false);

		int64_t min_val = INT64_MAX;
		for (int l = 0; l < robot_3_output.size(); l++) {
			if (robot_3_output[l].size() < min_val) {
				min_val = robot_3_output[l].size();
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
