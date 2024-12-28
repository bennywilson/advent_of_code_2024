#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
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

typedef string Path;
typedef vector<string> PathLists;
typedef vector<PathLists> SequencePath;

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

void find_numpad_paths(const char start_digit, const char end_digit, PathLists& s_paths) {
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

int64_t dir_changes(const string& path) {
	char start = path[0];
	int64_t count = 0;
	for (uint64_t i = 1; i < path.size(); i++) {
		if (path[i] != start) {
			count++;
			start = path[i];
		}
	}
	return count;
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

			/*** OLD ***/
			//sequences.push_back(cur_node.path);

			/***********/

		//	if (sequences.empty()) {
				sequences.push_back(cur_node.path);
			/*} else {
				//int cur_count = dir_changes(cur_node.path);
				//int existing_count = dir_changes(sequences[0]);
				{//if (cur_count < existing_count) {
					sequences[0] = cur_node.path;
				}

			}*/
			continue;
		}

		const Vec2& cur_pos = cur_node.pos;
		if (!cur_pos.valid_arrowpad_pos()) {
			continue;
		}
		if (cur_pos.x > end_pos.x && cur_pos.left().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.left(), cur_pos, cur_node.path + '<'));
		}
		if (cur_pos.y > end_pos.y && cur_pos.up().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.up(), cur_pos, cur_node.path + '^'));
		}

		if (cur_pos.x < end_pos.x && cur_pos.right().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.right(), cur_pos, cur_node.path + '>'));
		}
		if (cur_pos.y < end_pos.y && cur_pos.down().valid_arrowpad_pos()) {
			q.push(PathNode(cur_pos.down(), cur_pos, cur_node.path + 'v'));
		}

	}
}

/*
x ^ A | x ^ A
< v > | < v >
*/
void finalize_arrowpad(const SequencePath& src_seq, PathLists& out_paths) {
	PathLists path_1 = {src_seq[0]};
	PathLists path_2;

	static int done_once = 0;
//	string concat;/*
if (done_once == 1) {
		string concat;
		for (int i = 0; i < src_seq.size(); i++) {
			concat = concat + src_seq[i][0] + "A";
		}
		out_paths.push_back(concat);
		return;
	}
//	done_once = 1;
	for (uint64_t path_idx = 1; path_idx < src_seq.size(); path_idx++) {
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
	for (uint64_t i = 0; i < src2.size(); i++) {
		out_paths.push_back(src2[i] + 'A');
	}
}

unordered_map<string, SequencePath> cache;

void shrink_list(PathLists& paths) {
	size_t min_val = SIZE_MAX;
	size_t idx;
	vector<string>& src = paths;
	for (size_t l = 0; l < src.size(); l++) {
		size_t dir_change = dir_changes(src[l]);
		if (dir_change < min_val) {
			min_val = dir_change;
			idx = l;
		}
	/*	if (src[l].size() < min_val) {
			min_val = dir_changes[l].size();
			idx = l;
		}*/
	}

	paths[0] = paths[idx];
	paths.resize(1);
}

void part_one() {
	const string inputs[] = {
		"803A",
		"528A",
		"586A",
		"341A",
		"319A"
	};
	uint64_t complexity_sum = 0;
	for (uint64_t input_idx = 0; input_idx < 5; input_idx++) {
		const string& input = inputs[input_idx];

		PathLists paths1, paths2;
		{
			PathLists new_paths;
			find_numpad_paths('A', input[0], new_paths);
			vector<PathLists> s_all_path_lists = { new_paths };

			for (size_t i = 0; i < input.size() - 1; i++) {
				PathLists new_paths;
				find_numpad_paths(input[i], input[i + 1], new_paths);
				s_all_path_lists.push_back(new_paths);
			}
			finalize_arrowpad(s_all_path_lists, paths1);

			shrink_list(paths1);
		}

		for (uint64_t robot_idx = 0; robot_idx < 2; robot_idx++) {
			PathLists& src_pathlists = (paths1.size() > 0)?(paths1):(paths2);
			PathLists& dst_pathlists = (paths1.size() > 0)?(paths2):(paths1);

			cout << "Robot " << robot_idx << endl;
			for (const auto& cur_sequence : src_pathlists) {
				vector<string> sequences = {};
				find_arrowpad_paths('A', cur_sequence[0], sequences);
				SequencePath sSequences = { sequences };

				/*SequencePath test_seq = { sequences };
				for (size_t i = 0; i < cur_sequence.size() - 1; i++) { // Skip \0 at end
					vector<string> testsequences = {};
					find_arrowpad_paths(cur_sequence[i], cur_sequence[i + 1], testsequences);
					test_seq.push_back(testsequences);
				}*/

				size_t cmd_start = 0;

				size_t cmd_end = cur_sequence.find('A');
			//	SequencePath sSequences = { sequences };
				while (cmd_start < cur_sequence.length()) {
					string cache_val = cur_sequence.substr(cmd_start, cmd_end - cmd_start);
					if (std_contains(cache, cache_val)) {
					//	cout << "Found cached val " << cache_val << endl;
						sSequences.insert(sSequences.end(), cache[cache_val].begin(), cache[cache_val].end());

						/*SequencePath test_seq;
						for (int64_t i = cmd_start; cmd_end != cur_sequence.npos && i < cmd_end; i++) {
							vector<string> sequences = {};
							find_arrowpad_paths(cur_sequence[i], cur_sequence[i + 1], sequences);
							test_seq.push_back(sequences);
						}
						if (test_seq != cache[cache_val]) {
							SequencePath test_seq;
							for (int64_t i = cmd_start; cmd_end != cur_sequence.npos && i < cmd_end; i++) {
								vector<string> sequences = {};
								find_arrowpad_paths(cur_sequence[i], cur_sequence[i + 1], sequences);
								test_seq.push_back(sequences);
							}
						}*/

					} else {
						if (cache_val == "A") {
							static int lordy = 1;
							lordy++;
						}
						SequencePath local_seq;
						for (uint64_t i = cmd_start; cmd_end != (uint64_t)cur_sequence.npos && i < cmd_end; i++) {
								vector<string> sequences = {};
								find_arrowpad_paths(cur_sequence[i], cur_sequence[i + 1], sequences);
								local_seq.push_back(sequences);
							}

							//sSequences.push_back(sequences);
							if (cache_val.size() > 1) {
								cache[cache_val] = local_seq;
							}
							sSequences.insert(sSequences.end(), local_seq.begin(), local_seq.end());
					}
					
					cmd_start = cmd_end;
					cmd_end = cur_sequence.find('A', cmd_start + 1);
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

struct ArrowCache {
	ArrowCache(char _s, char _e, bool debug = false) :
		start(_s), end(_e) {
		if (debug)
		cout << "Arrow Caching " << _s << " " << _e << endl;
}
	char start;
	char end;
	bool operator==(const ArrowCache& op) const { return start == op.start && end == op.end; }
};

struct ArrowInfo {
	ArrowInfo() : count(0) {}
	ArrowInfo(uint64_t count, const string& seq) : count(0), sequence(seq) {}
	uint64_t count;
	string sequence;
};

namespace std {
	template<> struct hash<ArrowCache> {
		size_t operator()(const ArrowCache& r) const {
			size_t res = hash<char>{}(r.start) ^ hash<char>{}(r.end);
			return res;
		}
	};
};

string expand_sequence(char start, char end, unordered_map<ArrowCache, ArrowInfo>& arrow_cache) {
	const string& new_seq = arrow_cache[ArrowCache(start, end)].sequence;
	return new_seq;
}

/*
*				X  ^  A
*				<  v  >
* */
void part_two() {

	PathLists sequences;
	unordered_map<ArrowCache, ArrowInfo> arrow_cache;
	unordered_map<ArrowCache, ArrowInfo> numpad_cache;
	#define CACHE_PATH(A, B) \
		sequences.clear(); \
		find_arrowpad_paths(A, B, sequences); \
		shrink_list(sequences); \
		arrow_cache[ArrowCache(A, B)] = ArrowInfo(sequences.size(), sequences[0]);

	/*CACHE_PATH('A', '^');
	CACHE_PATH('A', '<');
	CACHE_PATH('A', 'v');
	CACHE_PATH('A', '>');
	arrow_cache[ArrowCache('A', 'A')] = ArrowInfo(1, "A");

	CACHE_PATH('^', 'A');
	CACHE_PATH('^', '>');	// > v vs    v>
	CACHE_PATH('^', 'v');
	CACHE_PATH('^', '<');
	arrow_cache[ArrowCache('^', '^')] = ArrowInfo(1, "A");


	CACHE_PATH('<', 'A');
	CACHE_PATH('<', '^');
	CACHE_PATH('<', 'v');
	CACHE_PATH('<', '>');
	arrow_cache[ArrowCache('<', '<')] = ArrowInfo(1, "A");

	CACHE_PATH('>', 'A');
	CACHE_PATH('>', '^');
	CACHE_PATH('>', 'v');
	CACHE_PATH('>', '<');
	arrow_cache[ArrowCache('>', '>')] = ArrowInfo(1, "A");

	CACHE_PATH('v', '^');
	CACHE_PATH('v', 'A');
	CACHE_PATH('v', '>');
	CACHE_PATH('v', '<');
	arrow_cache[ArrowCache('v', 'v')] = ArrowInfo(1, "A");
*/


		arrow_cache[ArrowCache('^', '^')] = ArrowInfo(1, "A");
		arrow_cache[ArrowCache('^', 'A')] = ArrowInfo(1, ">A");
		arrow_cache[ArrowCache('^', '<')] = ArrowInfo(1, "v<A");
		arrow_cache[ArrowCache('^', 'v')] = ArrowInfo(1, "vA");
		arrow_cache[ArrowCache('^', '>')] = ArrowInfo(1, "v>A");

		arrow_cache[ArrowCache('A', '^')] = ArrowInfo(1, "<A");
		arrow_cache[ArrowCache('A', 'A')] = ArrowInfo(1, "A");
		arrow_cache[ArrowCache('A', '<')] = ArrowInfo(1, "v<<A");
		arrow_cache[ArrowCache('A', 'v')] = ArrowInfo(1, "<vA");
		arrow_cache[ArrowCache('A', '>')] = ArrowInfo(1, "vA");

		arrow_cache[ArrowCache('<', '^')] = ArrowInfo(1, ">^A");
		arrow_cache[ArrowCache('<', 'A')] = ArrowInfo(1, ">>^A");
		arrow_cache[ArrowCache('<', '<')] = ArrowInfo(1, "A");
		arrow_cache[ArrowCache('<', 'v')] = ArrowInfo(1, ">A");
		arrow_cache[ArrowCache('<', '>')] = ArrowInfo(1, ">>A");

		arrow_cache[ArrowCache('v', '^')] = ArrowInfo(1, "^A");
		arrow_cache[ArrowCache('v', 'A')] = ArrowInfo(1, "^>A");
		arrow_cache[ArrowCache('v', '<')] = ArrowInfo(1, "<A");
		arrow_cache[ArrowCache('v', 'v')] = ArrowInfo(1, "A");
		arrow_cache[ArrowCache('v', '>')] = ArrowInfo(1, ">A");

		arrow_cache[ArrowCache('>', '^')] = ArrowInfo(1, "<^A");
		arrow_cache[ArrowCache('>', 'A')] = ArrowInfo(1, "^A");
		arrow_cache[ArrowCache('>', '<')] = ArrowInfo(1, "<<A");
		arrow_cache[ArrowCache('>', 'v')] =  ArrowInfo(1, "<A");
		arrow_cache[ArrowCache('>', '>')] = ArrowInfo(1, "A");

		numpad_cache[ArrowCache('0', '0')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('0', '1')] = ArrowInfo(1, "^<A");
		numpad_cache[ArrowCache('0', '2')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('0', '3')] = ArrowInfo(1, ">^A");
		numpad_cache[ArrowCache('0', '4')] = ArrowInfo(1, "^^<A");
		numpad_cache[ArrowCache('0', '5')] = ArrowInfo(1, "^^A");
		numpad_cache[ArrowCache('0', '6')] = ArrowInfo(1, ">^^A");
		numpad_cache[ArrowCache('0', '7')] = ArrowInfo(1, "^^^<A");
		numpad_cache[ArrowCache('0', '8')] = ArrowInfo(1, "^^^A");
		numpad_cache[ArrowCache('0', '9')] = ArrowInfo(1, ">^^^A");
		numpad_cache[ArrowCache('0', 'A')] = ArrowInfo(1, ">A");

		numpad_cache[ArrowCache('1', '0')] = ArrowInfo(1, ">vA");
		numpad_cache[ArrowCache('1', '1')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('1', '2')] = ArrowInfo(1, ">A");
		numpad_cache[ArrowCache('1', '3')] = ArrowInfo(1, ">>A");
		numpad_cache[ArrowCache('1', '4')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('1', '5')] = ArrowInfo(1, "^>A");
		numpad_cache[ArrowCache('1', '6')] = ArrowInfo(1, "^>>A");
		numpad_cache[ArrowCache('1', '7')] = ArrowInfo(1, "^^A");
		numpad_cache[ArrowCache('1', '8')] = ArrowInfo(1, "^^>A");
		numpad_cache[ArrowCache('1', '9')] = ArrowInfo(1, "^^>>A");
		numpad_cache[ArrowCache('1', 'A')] = ArrowInfo(1, ">>vA");
		
		numpad_cache[ArrowCache('2', '0')] = ArrowInfo(1, "<vA");
		numpad_cache[ArrowCache('2', '1')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('2', '2')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('2', '3')] = ArrowInfo(1, ">A");
		numpad_cache[ArrowCache('2', '4')] = ArrowInfo(1, "<^A");
		numpad_cache[ArrowCache('2', '5')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('2', '6')] = ArrowInfo(1, "^>A");
		numpad_cache[ArrowCache('2', '7')] = ArrowInfo(1, "<^^A");
		numpad_cache[ArrowCache('2', '8')] = ArrowInfo(1, "^^A");
		numpad_cache[ArrowCache('2', '9')] = ArrowInfo(1, "^^>A");
		numpad_cache[ArrowCache('2', 'A')] = ArrowInfo(1, "v>A");

		numpad_cache[ArrowCache('3', '0')] = ArrowInfo(1, "<vA");
		numpad_cache[ArrowCache('3', '1')] = ArrowInfo(1, "<<A");
		numpad_cache[ArrowCache('3', '2')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('3', '3')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('3', '4')] = ArrowInfo(1, "<<^A");
		numpad_cache[ArrowCache('3', '5')] = ArrowInfo(1, "<^A");
		numpad_cache[ArrowCache('3', '6')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('3', '7')] = ArrowInfo(1, "<<^^A");
		numpad_cache[ArrowCache('3', '8')] = ArrowInfo(1, "<^^A");
		numpad_cache[ArrowCache('3', '9')] = ArrowInfo(1, "^^A");
		numpad_cache[ArrowCache('3', 'A')] = ArrowInfo(1, "vA");

		numpad_cache[ArrowCache('4', '0')] = ArrowInfo(1, ">vvA");
		numpad_cache[ArrowCache('4', '1')] = ArrowInfo(1, "vA");
		numpad_cache[ArrowCache('4', '2')] = ArrowInfo(1, "v>A");
		numpad_cache[ArrowCache('4', '3')] = ArrowInfo(1, "v>>A");
		numpad_cache[ArrowCache('4', '4')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('4', '5')] = ArrowInfo(1, ">A");
		numpad_cache[ArrowCache('4', '6')] = ArrowInfo(1, ">>A");
		numpad_cache[ArrowCache('4', '7')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('4', '8')] = ArrowInfo(1, "^>A");
		numpad_cache[ArrowCache('4', '9')] = ArrowInfo(1, "^>>A");
		numpad_cache[ArrowCache('4', 'A')] = ArrowInfo(1, ">>vvA");

		numpad_cache[ArrowCache('5', '0')] = ArrowInfo(1, "vvA");
		numpad_cache[ArrowCache('5', '1')] = ArrowInfo(1, "<vA");
		numpad_cache[ArrowCache('5', '2')] = ArrowInfo(1, "vA");
		numpad_cache[ArrowCache('5', '3')] = ArrowInfo(1, "v>A");
		numpad_cache[ArrowCache('5', '4')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('5', '5')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('5', '6')] = ArrowInfo(1, ">A");
		numpad_cache[ArrowCache('5', '7')] = ArrowInfo(1, "<^A");
		numpad_cache[ArrowCache('5', '8')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('5', '9')] = ArrowInfo(1, "^>A");
		numpad_cache[ArrowCache('5', 'A')] = ArrowInfo(1, "vv>A");

		numpad_cache[ArrowCache('6', '0')] = ArrowInfo(1, "<vvA");
		numpad_cache[ArrowCache('6', '1')] = ArrowInfo(1, "<<vA");
		numpad_cache[ArrowCache('6', '2')] = ArrowInfo(1, "<vA");
		numpad_cache[ArrowCache('6', '3')] = ArrowInfo(1, "vA");
		numpad_cache[ArrowCache('6', '4')] = ArrowInfo(1, "<<A");
		numpad_cache[ArrowCache('6', '5')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('6', '6')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('6', '7')] = ArrowInfo(1, "<<^A");
		numpad_cache[ArrowCache('6', '8')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('6', '9')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('6', 'A')] = ArrowInfo(1, "vvA");

		numpad_cache[ArrowCache('7', '0')] = ArrowInfo(1, ">vvvA");
		numpad_cache[ArrowCache('7', '1')] = ArrowInfo(1, "vvA");
		numpad_cache[ArrowCache('7', '2')] = ArrowInfo(1, "vv>A");
		numpad_cache[ArrowCache('7', '3')] = ArrowInfo(1, "vv>>A");
		numpad_cache[ArrowCache('7', '4')] = ArrowInfo(1, "vA");
		numpad_cache[ArrowCache('7', '5')] = ArrowInfo(1, "v>A");
		numpad_cache[ArrowCache('7', '6')] = ArrowInfo(1, "v>>A");
		numpad_cache[ArrowCache('7', '7')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('7', '8')] = ArrowInfo(1, ">A");
		numpad_cache[ArrowCache('7', '9')] = ArrowInfo(1, ">>A");
		numpad_cache[ArrowCache('7', 'A')] = ArrowInfo(1, ">>vvvA");

		numpad_cache[ArrowCache('8', '0')] = ArrowInfo(1, "vvvA");
		numpad_cache[ArrowCache('8', '1')] = ArrowInfo(1, "<vvA");
		numpad_cache[ArrowCache('8', '2')] = ArrowInfo(1, "vvA");
		numpad_cache[ArrowCache('8', '3')] = ArrowInfo(1, "vv>A");
		numpad_cache[ArrowCache('8', '4')] = ArrowInfo(1, "<vA");
		numpad_cache[ArrowCache('8', '5')] = ArrowInfo(1, "vA");
		numpad_cache[ArrowCache('8', '6')] = ArrowInfo(1, "v>A");
		numpad_cache[ArrowCache('8', '7')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('8', '8')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('8', '9')] = ArrowInfo(1, ">A");
		numpad_cache[ArrowCache('8', 'A')] = ArrowInfo(1, "vvv>A");


		numpad_cache[ArrowCache('9', '0')] = ArrowInfo(1, "<vvvA");
		numpad_cache[ArrowCache('9', '1')] = ArrowInfo(1, "<<vvA");
		numpad_cache[ArrowCache('9', '2')] = ArrowInfo(1, "<vvA");
		numpad_cache[ArrowCache('9', '3')] = ArrowInfo(1, "vvA");
		numpad_cache[ArrowCache('9', '4')] = ArrowInfo(1, "<<vA");
		numpad_cache[ArrowCache('9', '5')] = ArrowInfo(1, "<vA");
		numpad_cache[ArrowCache('9', '6')] = ArrowInfo(1, "vA");
		numpad_cache[ArrowCache('9', '7')] = ArrowInfo(1, "<<A");
		numpad_cache[ArrowCache('9', '8')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('9', '9')] = ArrowInfo(1, "A");
		numpad_cache[ArrowCache('9', 'A')] = ArrowInfo(1, "vvvA");

		numpad_cache[ArrowCache('A', '0')] = ArrowInfo(1, "<A");
		numpad_cache[ArrowCache('A', '1')] = ArrowInfo(1, "^<<A");
		numpad_cache[ArrowCache('A', '2')] = ArrowInfo(1, "<^A");
		numpad_cache[ArrowCache('A', '3')] = ArrowInfo(1, "^A");
		numpad_cache[ArrowCache('A', '4')] = ArrowInfo(1, "^^<<A");
		numpad_cache[ArrowCache('A', '5')] = ArrowInfo(1, "<^^A");
		numpad_cache[ArrowCache('A', '6')] = ArrowInfo(1, "^^A");
		numpad_cache[ArrowCache('A', '7')] = ArrowInfo(1, "^^^<<A");
		numpad_cache[ArrowCache('A', '8')] = ArrowInfo(1, "<^^^A");
		numpad_cache[ArrowCache('A', '9')] = ArrowInfo(1, "^^^A");
		numpad_cache[ArrowCache('A', 'A')] = ArrowInfo(1, "A");

	const string inputs[] = {
		"803A",
		"528A",
		"586A",
		"341A",
		"319A"
	};
/*
	const string inputs[] = {
		"029A",
		"980A",
		"179A",
		"456A",
		"379A",
	};*/

	uint64_t complexity_sum = 0;
	for (uint64_t input_idx = 0; input_idx < 5; input_idx++) {
		const string& input = inputs[input_idx];

		string test_path = numpad_cache[ArrowCache('A', input[0])].sequence;
		test_path += numpad_cache[ArrowCache(input[0], input[1])].sequence;
		test_path += numpad_cache[ArrowCache(input[1], input[2])].sequence;
		test_path += numpad_cache[ArrowCache(input[2], input[3])].sequence;
		PathLists paths1 = { test_path };
		unordered_map<ArrowCache, uint64_t> map_1;
		map_1[ArrowCache('A', paths1[0][0])] = 1;
		for (uint64_t i = 0; i < paths1[0].size() - 1; i++) {
			map_1[ArrowCache(paths1[0][i], paths1[0][i + 1])]++;
		}
 
		unordered_map<ArrowCache, uint64_t> map_2;

		for (uint64_t robot_idx = 0; robot_idx < 25; robot_idx++) {
			unordered_map<ArrowCache, uint64_t>& src = (map_1.size() > 0)?(map_1):(map_2);
			unordered_map<ArrowCache, uint64_t>& dst = (map_1.size() > 0) ? (map_2) : (map_1);
			for (const auto& key: src) {
				uint64_t src_count = key.second;
				const string new_seq = expand_sequence(key.first.start, key.first.end, arrow_cache);
				if (key.first.start == key.first.end) {
					dst[ArrowCache('A', 'A')] += src_count;
					continue;
				}
				dst[ArrowCache('A', new_seq[0], false)] += src_count;
				for (uint64_t i = 0; i < new_seq.size() - 1; i++) {
					dst[ArrowCache(new_seq[i], new_seq[i+1], false)] += src_count;
				}
			}
			src.clear();
		}
		unordered_map<ArrowCache, uint64_t>& src = (map_1.size() > 0) ? (map_1) : (map_2);
		uint64_t total = 0;
		for (const auto& it : src) {
			total += it.second;
		}
		cout << "Total is " << total << endl;
		const uint64_t input_as_number = std::atoi(input.substr(0, 3).c_str());
		cout << "Numerical portion of input is " << input_as_number << endl;
		cout << "Score is " << total * input_as_number << endl;
		complexity_sum += total * input_as_number;
		src.clear();
	}

	cout << "Complexity sum is " << complexity_sum << endl;
}

/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";
	part_one();

	cout << "Part two..........................................................\n";

	part_two();
}
/*

// Valid 029A paths
029A
<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A

980A
<v<A>>^AAAvA^A<vA<AA>>^AvAA<^A>A<v<A>A>^AAAvA<^A>A<vA>^A<A>A

179A
<v<A>>^A<vA<A>>^AAvAA<^A>A<v<A>>^AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A

456A
<v<A>>^AA<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^A<A>A<v<A>A>^AAvA<^A>A

379A
<v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A

*/
// Too High 280096795185282
//  235047181384194
//	228800606998554 <- ?
//	229863014346622
