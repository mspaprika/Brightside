#pragma once

#include "Collidable.h"


class RenderContext;
struct StaticObject;


class CollisionManager
{
	struct StaticObject
	{
		Vector3f pos;
		float radius;

		StaticObject( const Vector3f& givenpPos, float givenRad ) : pos( givenpPos ), radius( givenRad ) {} // constructor
		StaticObject( const StaticObject&& obj ) : pos(obj.pos), radius(obj.radius) {} // move constructor

		StaticObject( const StaticObject& obj ) = default; // cannot delete copy constructor as I won't be able to access objects in the loop
		StaticObject& operator = ( const StaticObject& obj ) = delete; // delete assignment 
	};

	CollisionManager();
	~CollisionManager();

	PLAY_SINGLETON_INTERFACE( CollisionManager );

public:
	Collidable* CreateCollidable( GameObject* obj, const Vector3f& position, float radius, bool ignored = false );
	bool IsIntersectionBetweenAsteroids( const Vector3f& testPos, float radius, const std::vector< Vector3f >& positions ) const;
	bool IsObjectIntersectingObject( const Vector3f& testPos, float radius, const std::vector< Vector3f >& positions) const;
	const std::vector< StaticObject >& GetStaticObjects() const { return m_vStaticObjects; }

	void ReleaseCollidable( Collidable* c ); 
	void ReleaseAllCollidables();
	void UpdateCollisions();
	bool WillCollisionOccur( Collidable* col, const Vector3f& offset );
	void AddStaticObject( const Vector3f& pos, float radius );
	void CreateDebugSphere( Collidable* col );

private:
	float IsColliding( Collidable* obj1, Collidable* obj2 );
	std::vector< Collidable* > m_vCollidables;
	std::vector< StaticObject > m_vStaticObjects;
};