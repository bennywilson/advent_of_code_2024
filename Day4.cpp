#include <fstream>
#include <iostream>

using namespace std;

struct Vec2 {
	Vec2(const int in_x, const int in_y) :
		x(in_x),
		y(in_y) {}

	Vec2 operator+(const Vec2& op2) {
		return Vec2(x + op2.x, y + op2.y);
	}

	int x;
	int y;
};

int find_xmas(const string& game_board, const int board_width, string& cur_word, Vec2 pos, const Vec2& dir) {
	if (pos.x < 0 || pos.x >= board_width || pos.y < 0 || pos.y >= board_width) {
		return 0;
	}

	const string match_string = "XMAS";

	// String compare
	const int index = pos.x + pos.y * board_width;
	const char expected_char = match_string[cur_word.length() - 1];
	const char board_char = game_board[index];
	if (board_char != expected_char) {
		return 0;
	}

	// Xmas found?
	if (expected_char == 'S') {
		return 1;
	}

	cur_word += match_string[cur_word.length()];
	const int ret_val = find_xmas(game_board, board_width, cur_word, (pos + dir), dir);
	cur_word.pop_back();
	return ret_val;
}

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string game_board((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	cout << game_board << endl << endl;
	const int width = (int)game_board.find_first_of("\n");
	game_board.erase(std::remove(game_board.begin(), game_board.end(), '\n'), game_board.end());

	const int height = (int)(game_board.length())/ width;

	cout << endl << endl << "Width: " << width << " , Height: " << height << endl;

	int total_xmas = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vec2 pos(x, y);
			string cur_word = "X";

			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2(-1, -1));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2(-1,  0));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2(-1,  1));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2( 1, -1));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2( 1,  0));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2( 1,  1));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2( 0,  1));
			total_xmas += find_xmas(game_board, width, cur_word, pos, Vec2( 0, -1));
		}
	}

	cout << total_xmas << endl;
}
