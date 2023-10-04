#pragma once
#include "raylib.h"
#include "mushroom.hpp"
#include <iostream>
#include <algorithm>


class Renderer {
	float zoom;
	Texture2D map;
	float x_offset, y_offset;

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
		if (most_mushrooms < 0) {
			for (auto tile : tiles) {
				most_mushrooms = std::max(tile.mushroom_data.mushrooms.size(), most_mushrooms);
			}
		}
		srand(15);
		for (int i = 0; i < tiles.size(); i++) {
			draw_tile(tiles.at(i));
		}
	}

	void draw_mushroom_data(MushroomData& data) {
		for (int i = 0; i < 1000; i++) {
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

	size_t most_mushrooms = -1; // how many mushrooms the tile with the most mushrooms has
	void draw_tile(MapDataTile& tile) {
		float y_pos = latitude_to_screenspace(tile.latitude);
		float x_pos = longitude_to_screenspace(tile.longitude);

		float width = (longitude_to_screenspace(tile.width_longitude) - x_offset) / 2.0f; // no idea why you have to divide it by 2
		float height = -(latitude_to_screenspace(tile.height_latitude) - y_offset);

		char alpha = x_pos + y_pos / 5; //(((float)tile.mushroom_data.mushrooms.size() / (float)most_mushrooms) * 255);
		Color color = { rand() % 255, rand() % 255, rand() % 255, alpha};
		DrawRectangle(x_pos, y_pos, width, width, color);
	}

	float latitude_to_screenspace(float latitude) {
		float delta_lat = SOUTH_LAT - NORTH_LAT;
		float delta_pixel = SOUTH_PIXEL_Y - NORTH_PIXEL_Y;
		float pixel_per_latitude = delta_pixel / delta_lat;
		

		return (pixel_per_latitude * (latitude - NORTH_LAT)) * zoom + y_offset;
	}

	float longitude_to_screenspace(float longitude) {
		float delta_long = EAST_LONG - WEST_LONG;
		float delta_pixel = EAST_PIXEL_X - WEST_PIXEL_X;
		float pixel_per_longitude = delta_pixel / delta_long;
		

		return (pixel_per_longitude * (longitude - WEST_LONG)) * zoom + x_offset;
	}
};