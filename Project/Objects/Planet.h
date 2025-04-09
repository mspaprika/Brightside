#pragma once

#include "ObjectFactory/GameObject.h"


class RenderContext;

class Planet : public GameObject
{
public:
	Planet();
	~Planet() override;

	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_PLANET; }

private:

};