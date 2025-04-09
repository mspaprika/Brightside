#pragma once
#include "ExampleParticleFX.h"
#include "ObjectFactory/GameObject.h"


class RingBlast : public ParticleFX
{
public:
	RingBlast( ParticleType type, Vector3f pos, Vector3f right, int count, float life, Vector3f color = { 0.2f, 1.0f, 0.4f } );
	~RingBlast();

	void Update( float dT ) override;
	void GenerateParticlesBatch() override;

private:
	Vector3f m_right;
	u32 m_batchCount{ 0 };
};