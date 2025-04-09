#pragma once
#include "ObjectFactory/CollidableObject.h"


class RenderContext;

class Asteroid : public CollidableObject
{
public:
	Asteroid( const Matrix4x4f& world, AsteroidType type );

	~Asteroid() override;
	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	void ResolveCollision( GameObject* obj, float overlap ) override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_ASTEROID; };
	void BounceOff( GameObject* obj, float overlap ) override;
	AsteroidType GetNewType() const;
	void Explode();
	AssetType ConvertType( AsteroidType type );
	void SetDrawPosition( const Vector3f& pos ) { m_drawTransform.m_column[ 3 ] = { pos , 1 }; }
	void CreateRandomPowerUp() const;
	
private:
	Matrix4x4f m_drawTransform;
	AsteroidType m_asteroidType;
	float m_acceleration{ 0.0f };
	float m_minSpeed{ 1.0f };
	float m_rotAngle{ 0.0f };
	float m_separatioDistance{ 0.8f };
};