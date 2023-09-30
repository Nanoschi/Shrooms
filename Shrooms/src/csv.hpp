#pragma once
#include <fstream>
#include <array>
#include <vector>
#include <string>

template<size_t column_count>
class CSVTable {
public:
	std::vector<std::array<std::string, column_count>> content;
	size_t row_count;

	CSVTable() : row_count(0), content(std::vector<std::array<std::string, column_count>>()) {}

	void load_from_file(const char* path) {
		size_t line_count = 0;
		std::ifstream file(path);
		std::string line;
		std::getline(file, line); // ignore first line

		while (std::getline(file, line)) {
			add_row(line);
			line_count++;
		}
		row_count = line_count;
	}

	void clear() {
		content.clear();
		row_count = 0;
	}

private:

	void add_row(std::string& line) {
		std::array<std::string, column_count> row;
		int current_element = 0;
		int last = 0;
		bool in_str = false;

		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '"') {
				in_str = !in_str;
			}
			if (in_str) {
				continue;
			}

			if (line[i] == ',' || i == line.size() - 1) {
				row[current_element] = line.substr(last, i - last);
				last = i + 1;
				current_element++;
			}
		}
		content.push_back(row);

	}
};

