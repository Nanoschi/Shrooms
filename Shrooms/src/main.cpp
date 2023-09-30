#include <fstream>
#include <string>
#include <vector>
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

int main() {
	CSVTable<7> data;
	data.load_from_file("resources/mushrooms/spottings.csv");
	MushroomData mushrooms;
	mushrooms.load_from_csv(data);
	data.clear();
	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Shroom");
	

	Renderer renderer;
	renderer.load_map("resources/map/germany_map.png");

	while (!WindowShouldClose()) {
		BeginDrawing();

		apply_input(renderer);
		renderer.draw_app();
		renderer.draw_mushroom_data(mushrooms);

		DrawFPS(0, 0);

		EndDrawing();
	}
	CloseWindow();
	
}