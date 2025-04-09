#include "GameMath.h"
#include "Types.h"
#include "SpaceshipEmission.h"
#include "RenderContext.h"



SpaceshipEmission::SpaceshipEmission( ParticleType type, int count, Vector3f right, Vector3f pos, Vector3f velocity, float life, Vector3f color )
{
	// Make a CPU buffer of particles.
	m_particleBatch = count;
	m_lifetime = life;
	m_position = pos;
	m_velocity = velocity;
	m_right = right;
	m_color = color;

	GenerateFreeList();
	GenerateParticlesBatch();
	CreateParticleMaterial( type );
	CreateParticleBuffer();
}

SpaceshipEmission::~SpaceshipEmission()
{

}

void SpaceshipEmission::Update( float dT )
{
	GenerateNewParticles();
	for (u32 i = 0; i < m_particleCount; ++i)
	{
		UpdateParticleAcceleration( i );
		UpdateSingleParticle( i, dT );
		KillExpiredParticle( i );
	}
	Graphics::UpdateBuffer(m_particlesGPU, m_particlesCPU.data(), m_particlesCPU.size() * sizeof(Particle));
}

void SpaceshipEmission::GenerateNewParticles()
{
	u32 particles = m_particleCount - m_liveParticles;
	if ( particles > m_particleBatch )
	{
		GenerateParticlesBatch();
	}
}

void SpaceshipEmission::GenerateParticlesBatch()
{
	for ( u32 i = 0; i < m_particleBatch; i++ )
	{
		Vector3f position = RandomizePosition( m_position, m_right, m_velocity );
		Vector3f velocity = RandomizeVelocity( m_right, m_velocity );
		AddParticleToCPU( position, velocity, RandomFloatRange( 0.05f, 0.15f ), m_color);
	}
}

void SpaceshipEmission::UpdateParticleAcceleration( int index )
{
	Particle p = m_particlesCPU[ index ];

	// get offset from particle to origin (player pos)
	Vector3f offset = p.pos - m_position;
	Vector3f offsetNorm = normalize(offset);
	float distFromMiddle = dot(offset, m_velocity);

	m_acceleration = ( dot(offsetNorm, m_right) < 0) ? normalize( m_right * distFromMiddle * 0.1f ) : normalize( m_right * -distFromMiddle * 0.1f );
}

Vector3f SpaceshipEmission::RandomizePosition( Vector3f pos, Vector3f right, Vector3f forward )
{
	Vector3f particlePos = pos + forward * 0.5f + right * RandFloatNorm() * 0.2f;
	return particlePos;
}

Vector3f SpaceshipEmission::RandomizeVelocity( Vector3f right, Vector3f forward )
{
	Vector3f velocity = normalize( forward + RandFloatNorm() * right * 2.0f );
	return velocity;
}