#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace std::chrono;

/**
 *
 */
int main() {
	const auto start_time = high_resolution_clock::now();
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	uint64_t check_sum = 0;
	string line;

	while (std::getline(file, line)) {
		cout << line << endl;

		struct FreeBlockInfo {
			FreeBlockInfo(int in_index, int in_numfree) :
				index(in_index),
				num_free(in_numfree) {}
			int index;
			int num_free;
		};

		vector<uint16_t> disk_map;
		for (uint32_t i = 0; i < line.size(); i++) {
			uint16_t output = UINT16_MAX;
			const uint32_t count = line[i] - '0';
			if (!(i & 0x01)) {
				output = i >> 1;
			}
			for (uint32_t j = 0; j < count; j++) {
				disk_map.push_back(output);
			}
		}

		int64_t end = disk_map.size() - 1;
		const size_t max_size = (size_t)-1;
		for (auto it = disk_map.begin(); it != disk_map.end(); ++it) {
			if (*it == UINT16_MAX) {
				cout << ".";
			}
			else {
				cout << *it;
			}
		}

		while (end > 0) {

		    
		      while (end != max_size && disk_map[end] == UINT16_MAX) {
		        end--;
			}
		    size_t run_val = disk_map[end];
			
            size_t end_start = end;
            while (end_start != max_size && disk_map[end_start] != UINT16_MAX && disk_map[end_start] == run_val) {
		        end_start--;
			}
			if (end_start == max_size) {
			    end_start = 0;
			}
			
            const int needed_size = end - end_start;
            cout << "Needed size is " << needed_size << endl;
            size_t start = 1;
            size_t run_length = 1;
            bool block_found = false;
            while (start < end) {
                if (disk_map[start + run_length] != UINT16_MAX) {
                    start += run_length;
                    run_length = 1;
                } else {
        
            
                    if (run_length >= needed_size) {
                        block_found = true;
                        cout << "Block found at " << start << " run length is " << run_length << endl;
                        for (int i = 0; i < run_length; i++) {
                            disk_map[start+i+1] = disk_map[end_start + i + 1];
                            disk_map[end_start + i + 1] = UINT16_MAX;
                        }
                        break;
                    }
                                run_length++;
                }
            }
            cout << "Block found = " << block_found << endl;
	end = end_start;
      		for (auto it = disk_map.begin(); it != disk_map.end(); ++it) {
    			if (*it == UINT16_MAX) {
    				cout << ".";
    			} else {
    				cout << *it;
    			}
    		}
    		cout << endl;
		}


		for (auto it = disk_map.begin(); it != disk_map.end(); ++it) {
			if (*it == UINT16_MAX) {
				cout << ".";
			} else {
				cout << *it;
			}
		}
		cout << endl;

		for (uint32_t i = 0; i < (uint32_t)disk_map.size(); i++) {
			if (disk_map[i] == UINT16_MAX) {
				break;
			}
			check_sum += (uint64_t)i * disk_map[i];
		}
	}

	cout << "Check sum is " << check_sum << endl;
}
// 6283170117911