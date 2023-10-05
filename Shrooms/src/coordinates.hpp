#pragma once

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

// germany height/width ration is ~1.356
#define X_TILE_COUNT 10
#define Y_TILE_COUNT (int)(X_TILE_COUNT * 1.356)