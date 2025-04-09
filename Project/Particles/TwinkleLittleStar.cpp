#include "GameMath.h"
#include "Types.h"
#include "TwinkleLittleStar.h"
#include "AssetManager.h"
#include "RenderContext.h"



TwinkleLittleStar::TwinkleLittleStar( int count, Vector3f color )
{
	// Make a CPU buffer of particles.
	m_particleBatch = count;
	m_color = color;

	GenerateFreeList();
	GenerateParticlesBatch();
	m_materialId = AssetManager::Instance().GetTwinklyStarMaterial();
	CreateParticleBuffer();
}

TwinkleLittleStar::~TwinkleLittleStar()
{

}

void TwinkleLittleStar::Update( float dT )
{
	for (u32 i = 0; i < m_particleCount; ++i)
	{
		m_particlesCPU[ i ].life[ 0 ] += dT;
	}
	Graphics::UpdateBuffer(m_particlesGPU, m_particlesCPU.data(), m_particlesCPU.size() * sizeof( Particle ));
}

void TwinkleLittleStar::GenerateParticlesBatch()
{
	for ( u32 i = 0; i < m_particleBatch; i++ )
	{
		AddParticleToCPU(RandVector3f() * RandomFloatRange(25.f, 45.f), RandVector3f() * 0.1f, RandomFloatRange(0.05f, 0.10f), m_color);
	}
}



