#include "GameMath.h"
#include <random>



float RandFloatNorm()
{
	return ( (float)rand() / RAND_MAX ) * 2.0f - 1.0f;
}

float RandFloatAbs()
{
	return abs( RandFloatNorm() );
}

Vector3f RandVector3f()
{
	return Vector3f
	{
		RandFloatNorm(),
		RandFloatNorm(),
		RandFloatNorm()
	};
}

Vector3f RandVector3fAbs()
{
	return Vector3f
	{
		abs( RandFloatNorm() ),
		abs( RandFloatNorm() ),
		abs( RandFloatNorm() )
	};
}

Vector3f RandVectorXZ()
{
	return Vector3f
	{
		RandFloatNorm(),
		RandFloatNorm(),
		0.0f,
	};
}

float DampingFactor( float initialSpeed, float dT, float dampingCoefficient )
{
	return initialSpeed * exp( -dampingCoefficient * dT );
}

float RandomFloatRange( float a, float b ) 
{
	std::random_device rd;  // Obtain a random number from hardware
	std::mt19937 gen(rd()); // Seed the generator
	std::uniform_real_distribution<float> dis( a, b ); // Define the range

	return dis( gen ); // Generate and return a random float between 1 and 2
}

Vector3f RandomVectorIn2DPlane( const Vector3f& pos, const Vector3f& right )
{
	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis(0.0f, 2.0f * 3.14f);
	float angle = dis( gen );

	Vector3f normal = normalize( pos );
	Vector3f left = normalize( cross( right, normal) );
	Vector3f randomVector = std::cos(angle) * right + std::sin( angle ) * left;

	return normalize( randomVector );
}

int RandomNumberRange( int min, int max ) 
{
	// Create a random device and use it to seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	// Define the range distribution
	std::uniform_int_distribution< int > distr(min, max);

	// Generate and return a random number within the range
	return distr( gen );
}

bool AreAnyObjectsFacingSameDirection( const std::vector<Vector3f>& positions, Vector3f testPos, float dotLimit )
{
	for ( Vector3f pos : positions )
	{
		if ( dot(normalize(testPos), normalize(pos)) > dotLimit )
		{
			return true;
		}
	}
	return false;
}

Vector3f RotateAroundAxis( const Vector3f& vec, const Vector3f& axis, float angle ) 
{
	float rad = angle * 3.14f / 180.0f; // Convert angle to radians
	float cosAngle = std::cos(rad);
	float sinAngle = std::sin(rad);

	Vector3f normalizedAxis = normalize( axis );

	// Rodrigues' rotation formula
	Vector3f rotatedVec =
		vec * cosAngle +
		cross( vec, normalizedAxis ) * sinAngle +
		normalizedAxis * dot( vec, normalizedAxis ) * (1 - cosAngle);

	return rotatedVec;
}