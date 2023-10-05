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

		for (int i = 0; i < tiles.size(); i++) {
			draw_tile(tiles.at(i));
		}
	}

	void draw_mushroom_data(MushroomData& data) {
		for (int i = 0; i < 30000; i++) {
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

		float west_pixel = longitude_to_screenspace(WEST_LONG);
		float east_pixel = longitude_to_screenspace(EAST_LONG);
		float north_pixel = latitude_to_screenspace(NORTH_LAT);
		float south_pixel = latitude_to_screenspace(SOUTH_LAT);
		float width = (east_pixel - west_pixel) / X_TILE_COUNT;
		float height = (south_pixel - north_pixel) / Y_TILE_COUNT;

		if (!is_rect_on_screen(x_pos, y_pos, width, height)) {
			return;
		}

		char alpha = (char)(tile.mushroom_data.mushrooms.size() / (double)most_mushrooms * 255);
		Color color = { 255, 0, 0, alpha};
		DrawRectangle(x_pos, y_pos, width, height, color);
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

	bool is_rect_on_screen(int x, int y, int width, int height) {
		return (x <= GetScreenWidth()) && (y <= GetScreenHeight())
			&& (x + width >= 0) && (y + height >= 0);
	}

	bool is_pos_on_screen(int x, int y) {
		return is_rect_on_screen(x, y, 0, 0);
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