#pragma once
#include "ObjectFactory/GameObject.h"
#include "ExampleParticleFX.h"


class SpaceshipEmission : public ParticleFX
{
public:

	SpaceshipEmission( ParticleType type, int count, Vector3f right, Vector3f pos, Vector3f velocity, float life, Vector3f color = { 1.f, 1.f, 0.5f });
	~SpaceshipEmission();

	void Update( float dT ) override;
	void UpdateRight( Vector3f right ) { m_right = right; }
	Vector3f RandomizeVelocity( Vector3f right, Vector3f forward );
	Vector3f RandomizePosition( Vector3f pos, Vector3f right, Vector3f forward );
	void UpdateParticleAcceleration( int index );
	void GenerateNewParticles();
	void GenerateParticlesBatch() override;

private:
	Vector3f m_right;
};
