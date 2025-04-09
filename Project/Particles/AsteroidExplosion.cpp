#include "GameMath.h"
#include "Types.h"
#include "AsteroidExplosion.h"
#include "RenderContext.h"


AsteroidExplosion::AsteroidExplosion( ParticleType type, Vector3f pos, int count, float life, Vector3f color )
{
	// Make a CPU buffer of particles.
	m_particleBatch = count;
	m_lifetime = life;
	m_position = pos;
	m_color = color;

	GenerateFreeList();
	GenerateParticlesBatch();
	CreateParticleMaterial( type );
	CreateParticleBuffer();
}

AsteroidExplosion::~AsteroidExplosion()
{

}
 
void AsteroidExplosion::Update( float dT )
{
	ParticleFX::Update( dT );
}

void AsteroidExplosion::GenerateParticlesBatch()
{
	for ( u32 i = 0; i < m_particleBatch; i++ )
	{
		AddParticleToCPU( m_position, normalize( RandVector3f() ) * 1.5f, RandomFloatRange( 0.1f, 0.3f ), m_color);
	}
}