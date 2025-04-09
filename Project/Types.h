///////////////////////////////////////////////////////////////////////////
//	File		: Types.h
//	Platform	: All
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

#pragma once


constexpr float kMaxAsteroidRadius = 0.6f;
constexpr float kOffsetMultiplier = 1.1f;
constexpr float kPowerUpDuration = 10.0f;
constexpr float kHealthIncrease = 0.5f;
constexpr float kMaxHealth = 5.0f;


enum GameLevel
{
	LEVEL_1 = 1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_DEFAULT,

	TOTAL_LEVELS,
};

enum class AsteroidType
{
	TYPE_SMALL = 0,
	TYPE_MEDIUM,
	TYPE_LARGE,
	TYPE_DEBRIS_S,
	TYPE_DEBRIS_M,

	TOTAL_ASTEROID_TYPES
};

enum class VolcanoType
{
	TYPE_TALL = 0,
	TYPE_CRATER,

	TOTAL_VOLCANO_TYPES,
};

inline float g_vAsteroidRadius[ static_cast< int >(AsteroidType::TOTAL_ASTEROID_TYPES) ]
{
	kMaxAsteroidRadius / 3.0f , kMaxAsteroidRadius / 3.0f * 2.0f - 0.05f, kMaxAsteroidRadius, 0.1f
};

enum class GameObjectType
{
	TYPE_NONE = 0,
	TYPE_PLAYER ,
	TYPE_PLANET,
	TYPE_LASER,
	TYPE_ASTEROID,
	TYPE_ENEMY,
	TYPE_PARTICLE,
	TYPE_MISSILE,
	TYPE_VOLCANO,
	TYPE_VOLCANO_CRATER,
	TYPE_DEBRIS,
	TYPE_POWERUP,

	TOTAL_OBJ_TYPES
};

enum class AssetType 
{
	TYPE_PLAYER = 0,
	TYPE_PLANET,
	TYPE_ENEMY,
	TYPE_LASER,
	TYPE_MISSILE,
	TYPE_VOLCANO,
	TYPE_CRATER,
	TYPE_HEALTH,
	TYPE_SHIELD,
	TYPE_SPEED,
	TYPE_ASTEROID_S,
	TYPE_ASTEROID_M,
	TYPE_ASTEROID_L,
	TYPE_DEBRIS_S,
	TYPE_DEBRIS_M,

	TOTAL_ASSETS,
};

enum ShaderType
{
	PARTICLE_SHADER = 0,
	TWINKLY_STAR,
	SELF_ILLUMINATE,

	TOTAL_SHADERS,
};

enum class ParticleEffectType
{
	SHIP_EMISSION = 0,
	ASTEROID_COLLISION,
	RING_BLAST,

	TOTAL_EMISSION_TYPES
};

enum class Color
{
	WHITE = 0,
	L_PURPLE,
	D_PURPLE,
	L_BLUE,
	D_BLUE,
	L_GREEN,
	D_GREEN,
	L_YELLOW,
	D_YELLOW,
	L_PINK,
	D_PINK,

	TOTAL_COLOURS,
};

enum class PowerUpType
{
	TYPE_HEALTH = 0,
	TYPE_SHIELD,
	TYPE_SPEED,

	TOTAL_POWERUPS,
};

enum class ParticleType
{
	TYPE_STAR = 0,
	TYPE_DIRT,
	TYPE_SCORCH,
	TYPE_PIXEL,
	TYPE_SPARK,
	TYPE_EXPLOSION,
	TYPE_MAGIC_STAR,
	TYPE_FLARE,
	TYPE_SNOW,
	TYPE_FLUID,

	TOTAL_PARTICLE_TYPES,
};
