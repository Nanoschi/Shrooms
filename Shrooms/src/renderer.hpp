#pragma once
#include "raylib.h"
#include "mushroom.hpp"
#include <iostream>

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
		DrawCircle(x_pos, y_pos, radius, RED);
	}

	const float northern_lat = 54.91091764874563f;
	const float northern_pixel_y = 0.0f;
	const float southern_lat = 47.27024633137942f;
	const float southern_pixel_y = 4745.0f;

	float latitude_to_screenspace(float latitude) {
		float delta_lat = southern_lat - northern_lat;
		float delta_pixel = southern_pixel_y - northern_pixel_y;
		float pixel_per_latitude = delta_pixel / delta_lat;
		

		return (pixel_per_latitude * (latitude - northern_lat)) * zoom + y_offset;
	}

	const float east_long = 15.033333;
	const float east_pixel_x = 3499.0f;
	const float west_long = 5.866667f;
	const float west_pixel_x = 0.0f;

	float longitude_to_screenspace(float longitude) {
		float delta_lat = east_long - west_long;
		float delta_pixel = east_pixel_x - west_pixel_x;
		float pixel_per_longitude = delta_pixel / delta_lat;
		

		return (pixel_per_longitude * (longitude - west_long)) * zoom + x_offset;
	}
};