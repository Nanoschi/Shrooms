#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "csv.hpp"
#include "coordinates.hpp"


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

	void clear() {
		mushrooms.clear();
	}

private:
	static void set_mushroom_value(Mushroom& mushroom, const std::string& str_value, const char value_type) {
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
public:
	static Mushroom mushroom_from_row(std::array<std::string, 7>& row) {
		Mushroom mushroom;
		char current_field = ColumnTypes::OBSERVED;
		for (std::string item : row) {
			set_mushroom_value(mushroom, item, current_field);
			current_field++;
		}
		return mushroom;
	}

};

class MapDataTile{
public:
	MushroomData mushroom_data;
	float latitude;
	float longitude;
	float width_longitude;
	float height_latitude;

	MapDataTile(float _lat, float _long, float _width, float _height) {
		latitude = _lat;
		longitude = _long;
		width_longitude = _width;
		height_latitude = _height;
		mushroom_data = MushroomData();
	}

	void load_from_mushroom_data(MushroomData& data) {
		mushroom_data.mushrooms.resize(data.mushrooms.size());
		for (int i = 0; i < data.mushrooms.size(); i++) {
			if (is_pos_on_tile(data.mushrooms[i].latitude, data.mushrooms[i].longitude)) {
				mushroom_data.mushrooms[i] = data.mushrooms[i];
			}
		}
	}

	static void load_all_from_csv(CSVTable<7>& table, std::vector<MapDataTile>& tiles) {
		// creating the tiles
		tiles.clear();
		float tile_height = LAT_RANGE / Y_TILE_COUNT;
		float tile_width = LONG_RANGE / X_TILE_COUNT;
		for (int y = 0; y < Y_TILE_COUNT; y++) {
			for (int x = 0; x < X_TILE_COUNT; x++) {
				float latitude = (NORTH_LAT - (tile_height * y));
				float longitude = WEST_LONG + (tile_width * x);
				tiles.push_back(MapDataTile(latitude, longitude, tile_width, tile_height));
			}
		}

		// filling the tiles with data
		for (int i = 0; i < table.content.size(); i++) {
			// create mushroom
			Mushroom mushroom = MushroomData::mushroom_from_row(table.content.at(i));

			// calculate which tile the mushroom is in
			int tile_x = floor(((mushroom.longitude - WEST_LONG) / tile_width));
			int tile_y = floor(((mushroom.latitude - SOUTH_LAT) / tile_height));

			int idx = tile_y * X_TILE_COUNT + tile_x;
			if (idx < 0 || idx > tiles.size() - 1) {
				std::cout << "MEEP MEEP MEEP FEHLER !!!" << i << std::endl;
				continue;
			}

			// add mushroom to tile
			tiles[idx].mushroom_data.mushrooms.push_back(mushroom);
		}
		
	}

private:

	bool is_pos_on_tile(float _lat, float _long) {
		return (_lat <= latitude + height_latitude && _lat > latitude) 
			&& (_long <= longitude + width_longitude && _long > longitude);
	}

};
