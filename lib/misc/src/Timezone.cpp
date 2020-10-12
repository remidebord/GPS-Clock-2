#include "Timezone.h"

Timezone :: Timezone(void)
{
	
}

uint8_t Timezone :: offset(float latitude, float longitude)
{
	float row = 0;
	float column = 0;
	uint32_t tile = 0;
	
	uint32_t i = 0;
	uint32_t j = 0;
	
	uint8_t value = 0;
	
	// Compute column and row position
	column = ((float)(longitude + TIMEZONE_LON_MAX) / TIMEZONE_LON_RANGE) * TIMEZONE_COLUMNS;
	row = ((float)(TIMEZONE_LAT_MAX - latitude) / TIMEZONE_LAT_RANGE) * TIMEZONE_ROWS;
		
	// Get tile number
	tile = (row * TIMEZONE_COLUMNS) + column;
	
	// Search offset in grid array (RLE compressed)
	for(i = 0; i < GRID_LENGTH; i = i + 2)
	{
		j += grid[i];
		
		// Tile reached ?
		if(tile <= j)
		{
			value = grid[i+1];
			i = GRID_LENGTH;
		}
	}
	
	return value;
}
