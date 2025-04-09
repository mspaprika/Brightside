#include "GameMath.h"
#include "Types.h"
#include "RingBlast.h"
#include "RenderContext.h"


RingBlast::RingBlast( ParticleType type, Vector3f pos, Vector3f right, int count, float life, Vector3f color)
{
	// Make a CPU buffer of particles.
	m_particleBatch = count;
	m_lifetime = life;
	m_position = pos;
	m_right = right;
	m_color = color;
	m_batchCount++;

	GenerateFreeList();
	GenerateParticlesBatch();
	CreateParticleMaterial( type );
	CreateParticleBuffer();
}

RingBlast::~RingBlast()
{

}

void RingBlast::Update( float dT )
{
	ParticleFX::Update( dT );
	if (m_batchCount < 3) GenerateParticlesBatch();
	m_batchCount++;
}

void RingBlast::GenerateParticlesBatch()
{
	for (u32 i = 0; i < m_particleBatch; i++)
	{
		AddParticleToCPU( m_position, RandomVectorIn2DPlane(m_position, m_right) * 1.5f, RandomFloatRange(0.10f, 0.30f), m_color);
	}
}