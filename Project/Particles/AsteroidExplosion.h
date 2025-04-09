#pragma once
#include "ObjectFactory/GameObject.h"
#include "ExampleParticleFX.h"


class AsteroidExplosion : public ParticleFX
{
public:
	AsteroidExplosion( ParticleType type, Vector3f pos, int count, float life, Vector3f color = { 0.05f, 0.0f, 0.1f });
	~AsteroidExplosion();

	void Update( float dT ) override;
	void GenerateParticlesBatch() override;

private:
};