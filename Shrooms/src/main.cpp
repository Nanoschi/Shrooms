#include "renderer.hpp"
#include "mushroom.hpp"

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

	MapDataTile::load_all_from_csv(csv_data, tiles);

	csv_data.clear();
}

int main() {
	std::vector<MapDataTile> tiles;
	load_tiles("resources/mushrooms/test.csv", tiles);

	/*MushroomData md;
	CSVTable<7> csv;
	csv.load_from_file("resources/mushrooms/spottings.csv");*/
	//md.load_from_csv(csv);

	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Shroom");
	

	Renderer renderer;
	renderer.load_map("resources/map/germany_map.png");

	while (!WindowShouldClose()) {
		BeginDrawing();

		apply_input(renderer);
		renderer.draw_app();
		renderer.draw_tiles(tiles);
		//renderer.draw_mushroom_data(md);

		DrawFPS(0, 0);

		EndDrawing();
	}
	CloseWindow();
	
}