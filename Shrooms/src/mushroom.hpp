#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "csv.hpp"

struct MushroomInfo {
	  
};

struct Mushroom {
	float latitude;
	float longitude;
	std::string name;
	std::string lat_name;
	std::string observed_on;

	Mushroom(float _lat = 0,
		float _long = 0,
		std::string _name = "",
		std::string _lat_name = "",
		std::string _observed_on = ""
	) {
		latitude = _lat;
		longitude = _long;
		name = _name;
		lat_name = _lat_name;
		observed_on = _observed_on;
	}

};

class MushroomData {
	enum ColumnTypes : char {
		OBSERVED,
		PLACE_GUESS,
		LAT,
		LONG,
		COORDS_OBSCURED,
		SCIENT_NAME,
		NAME
	};


public:
	std::vector<Mushroom> mushrooms;

	MushroomData() : mushrooms(std::vector<Mushroom>()) {};

	void load_from_csv(CSVTable<7>& data) {
		for (auto element : data.content) {
			mushrooms.push_back(mushroom_from_row(element));
		}
	}

private:
	void set_mushroom_value(Mushroom& mushroom, const std::string& str_value, const char value_type) {
		switch (value_type) {
		case OBSERVED:
			mushroom.observed_on = str_value;
			break;
		case PLACE_GUESS:
			return; // TODO
		case LAT:
			mushroom.latitude = std::stof(str_value);
			break;
		case LONG:
			mushroom.longitude = std::stof(str_value);
			break;
		case COORDS_OBSCURED:
			return; // TODO
		case SCIENT_NAME:
			mushroom.lat_name = str_value;
		case NAME:
			mushroom.name = str_value;
		}
	}

	Mushroom mushroom_from_row(std::array<std::string, 7>& row) {
		Mushroom mushroom;
		char current_field = ColumnTypes::OBSERVED;
		for (std::string item : row) {
			set_mushroom_value(mushroom, item, current_field);
			current_field++;
		}
		return mushroom;
	}

	Mushroom mushroom_from_string(std::string str) {
		Mushroom mushroom;
		size_t last_comma = -1;
		char current_field = ColumnTypes::OBSERVED;
		bool in_str = false;

		for (size_t i = 0; i < str.size(); i++) {
			if (str.at(i) == '"') {
				in_str = !in_str;
			}
			if (in_str) {
				continue;
			}

			if (str.at(i) == ',' || i == str.size() - 1) {
				set_mushroom_value(mushroom, str.substr(last_comma + 1, i - last_comma - 1), current_field);
				last_comma = i;
				current_field++;
			}
		}
		return mushroom;

	}

};