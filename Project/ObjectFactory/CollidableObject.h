#pragma once

#include "GameObject.h"


class RenderContext;
class Collidable;


class CollidableObject : public GameObject
{
public:
	CollidableObject() { }
	~CollidableObject() {}

	virtual void ResolveCollision( GameObject* obj, float overlap ) {}
	virtual void BounceOff( GameObject* obj, float overlap ) {}
	void SetTransform( Matrix4x4f transform ) { m_transform = transform; };
	void RemoveCollidable() { m_pCol = nullptr; }

	void ManageActivity( float dT, float delay );
	bool IsActive() const { return m_active; }

protected:
	bool m_active{ false };
	float m_activityTimer{ 0.0f };
	Collidable* m_pCol{ nullptr };
};