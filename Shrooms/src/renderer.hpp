#pragma once
#include "raylib.h"
#include "mushroom.hpp"
#include <iostream>
#include <algorithm>


class Renderer {
	float zoom;
	float x_offset, y_offset;
	size_t most_mushrooms = 0;

	Texture2D map;
	Color bg_color;

public:
	Renderer() {
		zoom = 0.10f;
		x_offset = 0; y_offset = 0;

		map = Texture2D();
		bg_color = { 240, 240, 240, 255 };
	}

	void draw_app() {
		if (!IsWindowReady()) {
			return;
		}

		ClearBackground(bg_color);
		Vector2 map_pos = { x_offset, y_offset };
		DrawTextureEx(map, map_pos, 0, zoom, WHITE);
	}

	void draw_tiles(std::vector<MapDataTile>& tiles) {
		if (most_mushrooms == 0) {
			calculate_most_mushrooms(tiles);
		}

		if (zoom > 1) {
			for (int i = 0; i < tiles.size(); i++) {
				int x = longitude_to_screenspace(tiles[i].longitude);
				int y = latitude_to_screenspace(tiles[i].latitude);
				int width = long_to_width(tiles[i].width_longitude);
				int height = lat_to_height(tiles[i].height_latitude);
				if (!is_rect_on_screen(x, y, width, height)) {
					continue;
				}

				for (int j = 0; j < tiles[i].mushroom_data.mushrooms.size(); j++) {
					draw_mushroom(tiles[i].mushroom_data.mushrooms[j]);
				}
			}
		}
		else {
			for (int i = 0; i < tiles.size(); i++) {
				draw_tile(tiles.at(i));
			}
		}

		float x = longitude_to_screenspace(43.5);
		float y = latitude_to_screenspace(40);
		float w = long_to_width(0.4);
		float h = lat_to_height(0.4);
		std::cout << is_geo_rect_on_screen(43.5, 40, 0.4, 0.4) << " Rect: " << is_rect_on_screen(x, y, w, h) << std::endl;
		DrawRectangle(x, y, w, h, GREEN);
	}

	void draw_mushroom_data(MushroomData& data) {
		for (int i = 0; i < 10000; i++) {
			draw_mushroom(data.mushrooms[i]);
		}
	}

	void move_map(float dx, float dy) {
		x_offset += dx;
		y_offset += dy;
	}

	void change_zoom(float factor) {
		float zoom_start = zoom;
		zoom *= factor;

		move_map(
			(GetScreenWidth() + x_offset) * (zoom_start - zoom),
			(GetScreenHeight() + y_offset) * (zoom_start - zoom)
		);

	}

	void load_map(std::string path) {
		map = LoadTexture(path.c_str());
		center_map();
	}

	void center_map() {
		int width = GetScreenWidth();
		int height = GetScreenHeight();
		x_offset = (width / 2) - (map.width * zoom) / 2;
		y_offset = (height / 2) - (map.height * zoom) / 2;
	}

	~Renderer() {
		UnloadTexture(map);

	}
private:

	const int radius = 3;
	void draw_mushroom(Mushroom& mushroom) {
		float x_pos = longitude_to_screenspace(mushroom.longitude);
		float y_pos = latitude_to_screenspace(mushroom.latitude);

		if (x_pos >= 0 && x_pos <= GetScreenWidth() && y_pos >= 0 && y_pos <= GetScreenHeight()) {
			DrawCircle(x_pos, y_pos, radius, RED);
		}
	}

	void draw_tile(MapDataTile& tile) {
		float y_pos = latitude_to_screenspace(tile.latitude);
		float x_pos = longitude_to_screenspace(tile.longitude);

		float width = long_to_width(tile.width_longitude);
		float height = lat_to_height(tile.height_latitude);

		char alpha = (char)(tile.mushroom_data.mushrooms.size() / (double)most_mushrooms * 255);
		Color color = { 255, 0, 0, alpha};
		DrawRectangle(x_pos, y_pos, width, height, color);
	}

	// conversion functions

	float latitude_to_screenspace(float latitude) {
		return (PIXEL_PER_LAT * (latitude - SOUTH_LAT)) * zoom + y_offset;
	}

	float longitude_to_screenspace(float longitude) {
		return (PIXEL_PER_LONG * (longitude - WEST_LONG)) * zoom + x_offset;
	}

	int long_to_width(float long_width) {
		return long_width * PIXEL_PER_LONG * zoom;
	}

	int lat_to_height(float lat_height) {
		return lat_height * PIXEL_PER_LAT * zoom;
	}

	bool is_rect_on_screen(int x, int y, int width, int height) {
		return is_pos_on_screen(x, y) || is_pos_on_screen(x, y + height)
			|| is_pos_on_screen(x + width, y) || is_pos_on_screen(x + width, y + height);
	}

	bool is_geo_rect_on_screen(float longitude, float latitude, float width, float height) {
		return is_rect_on_screen(
			longitude_to_screenspace(longitude),
			latitude_to_screenspace(latitude),
			long_to_width(width),
			lat_to_height(height)
		);
	}

	bool is_pos_on_screen(int x, int y) {
		return x >= x_offset && x <= GetScreenWidth() + x_offset
			&& y >= y_offset && y <= GetScreenHeight() + y + y_offset;
	}

	void calculate_most_mushrooms(std::vector<MapDataTile>& tiles) {
		size_t most = 0;
		for (int i = 0; i < tiles.size(); i++) {
			most = tiles[i].mushroom_data.mushrooms.size() > most ? tiles[i].mushroom_data.mushrooms.size() : most;
		}
		most_mushrooms = most;
		std::cout << "MOST:  ads asd asd as da a: " << most << std::endl;
	}
};