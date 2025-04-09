#pragma once
#include "ObjectFactory/CollidableObject.h"


class RenderContext;

class Laser : public CollidableObject
{
public:
	Laser( const Matrix4x4f& world );

	~Laser() override;
	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	void ResolveCollision( GameObject* obj, float overlap ) override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_LASER; };

	void ManageLifetime( float dT );
	
private:
	float m_life{ 0.0f };
	float m_maxLife{ 10.0f };
};