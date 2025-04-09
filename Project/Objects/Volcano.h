#pragma once

#include "ObjectFactory/CollidableObject.h"


class RenderContext;

class Volcano : public CollidableObject
{
public:
	Volcano( VolcanoType type, Matrix4x4f world );
	~Volcano() override;

	void ResolveCollision( GameObject* obj, float overlap ) override {}
	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_VOLCANO; }
	AssetType ConvertType( VolcanoType type );

private:

};