#pragma once
#include "ObjectFactory/GameObject.h"
#include "ExampleParticleFX.h"


class TwinkleLittleStar : public ParticleFX
{
public:

	TwinkleLittleStar(int count, Vector3f color = { 1.0f, 1.0f, 1.0f });
	~TwinkleLittleStar();

	void Update( float dT ) override;
	void GenerateParticlesBatch() override;

private:

};
