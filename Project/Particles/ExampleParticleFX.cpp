#include "GameMath.h"
#include "Types.h"
#include "ExampleParticleFX.h"
#include "AssetManager.h"
#include "RenderContext.h"



ParticleFX::~ParticleFX()
{

}

void ParticleFX::CreateParticleMaterial( ParticleType type )
{
	m_materialId = AssetManager::Instance().GetParticleMaterial( type );
}

void ParticleFX::CreateParticleBuffer()
{
	// Make a GPU block of particles as a Structured buffer
	Graphics::BufferDesc desc;
	desc.SetDynamicStructuredBuffer< Particle >((u32)m_particlesCPU.size(), m_particlesCPU.data());
	desc.m_pDebugName = "ParticleBuffer";
	m_particlesGPU = Resources::CreateAsset< Graphics::Buffer >(desc);
}

void ParticleFX::AddParticleToCPU( Vector3f pos, Vector3f vel, float scale, Vector3f color )
{
	int index = m_freeList.back();
	m_particlesCPU[ index ].pos = pos;
	m_particlesCPU[ index ].vel = vel;
	m_particlesCPU[ index ].scale = scale;
	m_particlesCPU[ index ].colour = color;
	m_particlesCPU[ index ].life[ 0 ] = 0.0f;
	m_particlesCPU[ index ].life[ 1 ] = m_lifetime;

	CountParticles( 1 );
	m_freeList.pop_back();
}

void ParticleFX::GenerateFreeList()
{
	m_particlesCPU.resize( m_particleCount );
	m_freeList.reserve( m_particleCount );

	for ( int i = m_particleCount; i > -1; i-- )
	{
		m_freeList.push_back( i );
	}
}

void ParticleFX::GenerateParticlesBatch()
{
	for (u32 i = 0; i < m_particleBatch; i++)
	{
		AddParticleToCPU( m_position, m_velocity, 0.15f );
	}
}

void ParticleFX::Update( float dT )
{
	// Animate the particles on the CPU.
	for (u32 i = 0; i < m_particleCount; ++i)
	{
		UpdateSingleParticle( i, dT );
		KillExpiredParticle( i );
	}
	// Issue a command to update the particles.
	Graphics::UpdateBuffer( m_particlesGPU, m_particlesCPU.data(), m_particlesCPU.size() * sizeof(Particle) );
}

void ParticleFX::UpdateSingleParticle( int index, float dT )
{
	if ( m_particlesCPU[ index ].life[ 0 ] >= 0.f )
	{
		m_particlesCPU[ index ].vel += m_acceleration;
		m_particlesCPU[ index ].pos += m_particlesCPU[ index ].vel * dT;
		m_particlesCPU[ index ].life[ 0 ] += dT;
	}
}

void ParticleFX::KillExpiredParticle( int index )
{
	if (m_particlesCPU[ index ].life[ 0 ] > m_lifetime)
	{
		m_particlesCPU[ index ].life[ 0 ] = -1;
		m_freeList.push_back( index );
		CountParticles( -1 );
	}
}

void ParticleFX::Render( RenderContext& ctx ) const
{
	if ( ctx.GetRenderPassType() == RenderPassType::kForwardPass )
	{
		ParticleRenderCommand cmd;
		cmd.worldMatrix = m_transform;
		cmd.particleCount = m_particleCount;
		cmd.materialId = m_materialId;
		cmd.particleBufferId = m_particlesGPU;
		
		ctx.RenderParticles( cmd );		
	}
}
