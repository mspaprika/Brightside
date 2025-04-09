#pragma once
#include "ObjectFactory/GameObject.h"


class RenderContext;

class Debris : public GameObject
{
public:
	Debris( const Matrix4x4f& world, AsteroidType type );
	~Debris() override;
	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_DEBRIS; };

	AssetType ConvertType( AsteroidType type ) const;

private:
	AsteroidType m_asteroidType;
};