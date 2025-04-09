#pragma once
#include "../Play3DLib/Play3d.h"

#include "Types.h"

#include <concepts>
#include <memory>

using namespace Play3d;


inline Vector3f g_vParticleColours[ static_cast< int >(Color::TOTAL_COLOURS) ]
{
	Vector3f(1.0f, 1.0f, 1.0f), // white
	Vector3f(0.7f, 0.5f, 1.0f), // l purp 
	Vector3f(0.05f, 0.0f, 0.1f), // d purp 
	Vector3f(0.7f, 1.0f, 1.0f),  // l blue
	Vector3f(0.0f, 0.0f, 0.5f),  // d blue 
	Vector3f(0.2f, 1.0f, 0.4f),  // l green
	Vector3f(0.0f, 0.5f, 0.0f),  // d green 
	Vector3f(1.f, 1.f, 0.5f), // --> l yell
	Vector3f(1.f, 0.8f, 0.0f), // --> d yell 
	Vector3f(1.f, 0.5f, 1.0f), // --> l pink 
	Vector3f(1.f, 0.0f, 0.5f), // --> d pink 
};

constexpr u32 kNumLights = 4;

Vector3f RandVector3f();
Vector3f RandVectorXZ();
float RandFloatNorm();
float DampingFactor( float initialSpeed, float time, float dampingCoefficient );
Vector3f RandVector3fAbs();
float RandFloatAbs();
float RandomFloatRange( float a, float b );
int RandomNumberRange( int min, int max );
Vector3f RandomVectorIn2DPlane( const Vector3f& pos, const Vector3f& right );
bool AreAnyObjectsFacingSameDirection( const std::vector<Vector3f>& positions, Vector3f testPos, float dotLimit );
Vector3f RotateAroundAxis( const Vector3f& vec, const Vector3f& axis, float angle );