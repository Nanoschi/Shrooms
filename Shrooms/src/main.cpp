#include <fstream>
#include <string>
#include <vector>
#include "renderer.hpp"
#include "mushroom.hpp"

// germany height/width ration is ~1.356
#define X_TILE_COUNT 10
#define Y_TILE_COUNT (int)(X_TILE_COUNT * 1.356)

void apply_input(Renderer& renderer) {
	Vector2 delta_mouse = GetMouseDelta();

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		renderer.move_map(delta_mouse.x, delta_mouse.y);
	}
	if (GetMouseWheelMove() > 0) {
		renderer.change_zoom(1.2f);
	}
	else if (GetMouseWheelMove() < 0) {
		renderer.change_zoom(0.8f);
	}

	if (IsKeyPressed(KEY_ENTER)) {
		renderer.center_map() ;
	}
	
}

void load_tiles(const char* path, std::vector<MapDataTile>& tiles) {
	std::cout << "Loading mushroom CSV Data..." << std::endl;
	CSVTable<7> csv_data;
	csv_data.load_from_file(path);

	std::cout << "Converting CSV to mushrooms..." << std::endl;
	MushroomData all_mushrooms;
	all_mushrooms.load_from_csv(csv_data);

	std::cout << "Populating tiles with data..." << std::endl;
	float tile_height = Y_TILE_COUNT / LAT_RANGE;
	float tile_width = X_TILE_COUNT / LONG_RANGE;
	
	for (int y = 0; y < Y_TILE_COUNT; y++) {
		for (int x = 0; x < X_TILE_COUNT; x++) {
			printf("(%d/%d)\n", y * X_TILE_COUNT + x, X_TILE_COUNT * Y_TILE_COUNT);
			float latitude = NORTH_LAT - (tile_height * y);
			float longitude = WEST_LONG + (tile_height * x);
			auto tile = MapDataTile(latitude, longitude, tile_width, tile_height);
			tile.load_from_mushroom_data(all_mushrooms);
			tiles.push_back(tile);
		}
	}

	csv_data.clear();
	all_mushrooms.clear();
}

int main() {
	std::vector<MapDataTile> tiles;
	load_tiles("resources/mushrooms/spottings.csv", tiles);

	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Shroom");
	

	Renderer renderer;
	renderer.load_map("resources/map/germany_map.png");

	while (!WindowShouldClose()) {
		BeginDrawing();

		apply_input(renderer);
		renderer.draw_app();
		//renderer.draw_mushroom_data(mushrooms);

		DrawFPS(0, 0);

		EndDrawing();
	}
	CloseWindow();
	
}