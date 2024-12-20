#include <conio.h>
#include <chrono>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <sstream>
#include <stdio.h>
#include <windows.h>

using namespace std;
using namespace std::chrono;

const uint32_t g_board_width = 101;
const uint32_t g_board_height = 103;
const uint32_t g_middle_row = g_board_height / 2;
const uint32_t g_middle_column = g_board_width / 2;

/**
 *	Global Functions
 */
struct Vec2 {
	Vec2() :
		x(0),
		y(0) {}

	Vec2(const double in_x, const double in_y) :
		x(in_x),
		y(in_y) {}

	Vec2 operator+(const Vec2& op2) const {
		return Vec2(x + op2.x, y + op2.y);
	}

	Vec2 operator-(const Vec2& op2) const {
		return Vec2(x - op2.x, y - op2.y);
	}

	Vec2 operator*(const double op2) const {
		return Vec2(op2 * x, op2 * y);
	}

	bool operator==(const Vec2& op2) {
		return x == op2.x && y == op2.y;
	}

	bool operator!=(const Vec2& op2) const {
		return x != op2.x || y != op2.y;
	}

	double x;
	double y;
};

void print_board(const bool clear_console, unordered_map<uint64_t, uint64_t>& robots) {
//	_setmode(_fileno(stdout), _O_U16TEXT); // <=== Windows madness

	if (clear_console) {
		//	SetConsoleOutputCP(1200);
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		//	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
			//_setmode(_fileno(stdout), _O_WTEXT);
	/***/
			/*HANDLE hcsb = CreateFileA("CONOUT$", GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);*/
				// Populate cfi with the screen buffer's current font info
			/*	GetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);

				// Modify the font size in cfi
				cfi.dwFontSize.X *= 52;
				cfi.dwFontSize.Y *= 62;
		cfi.FontFamily= 99;
				// Use cfi to set the screen buffer's new font
				SetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);*/

				//	CloseHandle(hcsb);
					/***/
		DWORD mode = 0;
		if (!GetConsoleMode(hStdOut, &mode)) {
			return;
		}

		// Hold original mode to restore on exit to be cooperative with other command-line apps.
		const DWORD originalMode = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

		// Try to set the mode.
		if (!SetConsoleMode(hStdOut, mode)) {
			return;
		}

		// Write the sequence for clearing the display.
		DWORD written = 0;
		PCWSTR sequence = L"\x1b[2J";
		if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL)) {
			SetConsoleMode(hStdOut, originalMode);
			return;
		}

		sequence = L"\x1b[3J";
		if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL)) {
			SetConsoleMode(hStdOut, originalMode);
			return;

		}
		sequence = L"\x1b[H";
		if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL)) {
			SetConsoleMode(hStdOut, originalMode);
			return;

		}
		SetConsoleMode(hStdOut, originalMode);
	}

	string the_output;
	for (int64_t y = 0; y < g_board_height; y++) {
		for (int64_t x = 0; x < g_board_width; x++) {
			if (robots[x + y * g_board_width] > 0) {
				the_output += "@";
			}
			else {
				the_output += " ";
			}
		}
		the_output += "\n";
	}
	cout << the_output.c_str() << endl;
	//	printf("%s", the_output.c_str());

}
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
	double count = 7480.0f;
	bool bfreeze_next = false;
	double delta = 0.001f;

	while (true) {
		count += delta;
		robot_idx.clear();
		for (int i = 0; i < robots.size(); i++) {
			const Vec2 pos = robots[i].pos;
			const Vec2 vel = robots[i].vel;

			const Vec2 travel_vec = vel * (sim_length + count);
			Vec2 new_pos = pos + travel_vec;
			if (new_pos.x < 0) {
				new_pos.x = g_board_width - (g_board_width - (int64_t)new_pos.x) % g_board_width;
			}
			new_pos.x = (int64_t)new_pos.x % g_board_width;

			if (new_pos.y < 0) {
				new_pos.y = g_board_height - (g_board_height - (int64_t)new_pos.y) % g_board_height;
			}
			new_pos.y = (int64_t)new_pos.y % g_board_height;

			if (new_pos.x == g_middle_column || new_pos.y == g_middle_row) {
				continue;
			}

			if (new_pos.x < g_middle_column) {
				if (new_pos.y < g_middle_row) {
					tl_quad++;
				} else {
					bl_quad++;
				}
			} else {
				if (new_pos.y < g_middle_row) {
					tr_quad++;
				}
				else {
					br_quad++;
				}
			}

			const uint64_t board_idx = new_pos.x + new_pos.y * g_board_width;
			robot_idx[board_idx]++;
		}

		bool bDrawIt = true;

		if (bDrawIt) {
			print_board(true, robot_idx);
		}


		if (bfreeze_next) {
			getchar();
		}

		if (delta < 0.001) {
			delta = delta * 2.0;
			if (delta > 0.001)
				delta = 0.001;
		}

		for (uint64_t y = 0; y < g_board_height; y++) {
			int cur_span = 0;
			for (uint64_t x = 0; x < g_board_width; x++) {
				const uint64_t board_idx = x + y * g_board_width;
				if (robot_idx[board_idx] > 0) {
					cur_span++;
				}
				else {
				}
			}
			if (cur_span > 20) {
				longest_span = cur_span;
				for (uint64_t x = 0; x < g_board_width; x++) {
					int cur_span = 0;
					for (uint64_t y = 0; y < g_board_height; y++) {
						const uint64_t board_idx = x + y * g_board_width;
						if (robot_idx[board_idx] > 0) {
							cur_span++;
						}
					}
					if (cur_span > 20) {
						delta = 0.00025;
				//		cout << "Christmas tree at " << sim_length + count << " seconds" << endl;
						break;
					}
				}
			}
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
