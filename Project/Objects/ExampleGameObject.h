#pragma once

#include "ObjectFactory/CollidableObject.h"

///////////////////////////////////////////////////////////////////////////
//	File		: ExampleGameObject.h
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

class Collidable;

class ExampleGameObject : public GameObject
{
public:
	ExampleGameObject( Collidable* col );
	 ~ExampleGameObject() override;

	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_NONE; };
	
private:
	Collidable* m_pColidable{ nullptr };
};