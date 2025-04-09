#pragma once
#include "ObjectFactory/CollidableObject.h"


class RenderContext;

class Missile : public CollidableObject
{
public:
	Missile( const Matrix4x4f& world );

	~Missile() override;
	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	void ResolveCollision( GameObject* obj, float overlap ) override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_MISSILE; };
	void PlanetCollision();

private:
	float m_rotAngle{ 0.1f };
	float m_rotSpeed{ 3.0f };
	bool m_exploded{ false };
};