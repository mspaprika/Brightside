
///////////////////////////////////////////////////////////////////////////
//	File		: ExampleParticleFX.h
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "ObjectFactory/GameObject.h"


class RenderContext;


class ParticleFX : public GameObject
{
public:
	
	struct Particle
	{
		float scale;
		Vector3f pos;
		Vector3f vel;
		Vector3f colour;
		float life[ 4 ]; // extend
	};

	ParticleFX() {}
	~ParticleFX() override;

	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_PARTICLE; };

	void UpdatePosition( Vector3f pos ) { m_position = pos; }
	void UpdateVelocity( Vector3f vel ) { m_velocity = vel; }
	void CreateParticleMaterial( ParticleType type );
	void CreateParticleBuffer();
	void AddParticleToCPU( Vector3f pos, Vector3f vel, float scale, Vector3f color = { 1.f, 1.f, 1.f });
	void GenerateFreeList();
	void UpdateSingleParticle( int index, float dT );
	void KillExpiredParticle( int index );
	void UpdateBatch(u32 count) { m_particleBatch = count; }
	void CountParticles( int delta ) { m_liveParticles += delta; }
	virtual void GenerateParticlesBatch();

protected:
	std::vector< Particle > m_particlesCPU;
	std::vector< int > m_freeList;
	Graphics::BufferId m_particlesGPU;
	Vector3f m_acceleration{ 0.0f };
	Vector3f m_velocity{ 0.0f };
	Vector3f m_position{ 0.0f };
	u32 m_particleCount{ 1000 };
	u32 m_particleBatch{ 0 };
	int m_liveParticles{ 0 };
	float m_lifetime{ 0.0f };
	Vector3f m_color;
};