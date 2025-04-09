#pragma once



enum LevelConst
{
	ENEMY_SPEED,
	SHOOT_INTERVAL,
	VOLCANO_COUNT,
	CRATER_COUNT,
	ASTEROID_COUNT,
};

std::tuple< float, u16, u8, u8, u8 > g_levelConstants[ TOTAL_LEVELS ]
{
	{ 0.0f, 0, 0, 0, 0 },
	{ 0.5f, 400, 2, 1, 1 },
	{ 0.7f, 300, 3, 1, 2 },
	{ 0.9f, 200, 3, 1, 3 },
	{ 1.0f, 100, 3, 1, 3 },
};

