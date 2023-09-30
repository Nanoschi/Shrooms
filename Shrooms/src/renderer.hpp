#pragma once
#include "raylib.h"
#include "mushroom.hpp"
#include <iostream>

#define NORTH_LAT 54.91091764874563f
#define NORTH_PIXEL_Y 0.0f
#define SOUTH_LAT 47.27024633137942f
#define SOUTH_PIXEL_Y 4745.0f

#define EAST_LONG 15.033333f
#define EAST_PIXEL_X 3499.0f
#define WEST_LONG 5.866667f
#define WEST_PIXEL_X 0.0f

#define LAT_RANGE (NORTH_LAT - SOUTH_LAT)
#define LONG_RANGE (EAST_LONG - WEST_LONG)

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

	float latitude_to_screenspace(float latitude) {
		float delta_lat = SOUTH_LAT - NORTH_LAT;
		float delta_pixel = SOUTH_PIXEL_Y - NORTH_PIXEL_Y;
		float pixel_per_latitude = delta_pixel / delta_lat;
		

		return (pixel_per_latitude * (latitude - NORTH_LAT)) * zoom + y_offset;
	}

	float longitude_to_screenspace(float longitude) {
		float delta_lat = EAST_LONG - WEST_LONG;
		float delta_pixel = EAST_PIXEL_X - WEST_PIXEL_X;
		float pixel_per_longitude = delta_pixel / delta_lat;
		

		return (pixel_per_longitude * (longitude - WEST_LONG)) * zoom + x_offset;
	}
};