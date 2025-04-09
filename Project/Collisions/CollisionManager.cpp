#include "GameMath.h"
#include "Types.h"
#include "ObjectFactory/GameObject.h"
#include "ObjectFactory/CollidableObject.h"
#include "ObjectFactory/GameObjectManager.h"
#include "Objects//ExampleGameObject.h"
#include "CollisionManager.h"



PLAY_SINGLETON_IMPL( CollisionManager );

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

Collidable* CollisionManager::CreateCollidable( GameObject* obj, const Vector3f& position, float radius, bool ignored )
{
	Collidable* c = new Collidable( obj, position, radius, ignored );
	m_vCollidables.push_back( c );
	return c;
}

void CollisionManager::UpdateCollisions()
{
	for (int i = 0; i < m_vCollidables.size() - 1; i++)
	{
		for (int j = i + 1; j < m_vCollidables.size(); j++)
		{
			Collidable* c1 = m_vCollidables[ i ];
			Collidable* c2 = m_vCollidables[ j ];

			float overlap = IsColliding( c1, c2 );

			if (overlap > 0)
			{			
				CollidableObject* colObj1 = ( CollidableObject* )c1->GetGameObject();
				CollidableObject* colObj2 = ( CollidableObject* )c2->GetGameObject();

				colObj1->ResolveCollision( c2->GetGameObject(), overlap );
				colObj2->ResolveCollision( c1->GetGameObject(), overlap );
			}
		}
	}
}

float CollisionManager::IsColliding( Collidable* col1, Collidable* col2 )
{
	Vector3f offset = col1->GetBoundSphere().position - col2->GetBoundSphere().position;
	float distance = length( offset );
	float radialSum = col1->GetBoundSphere().radius + col2->GetBoundSphere().radius;

	return radialSum - distance;
}

void CollisionManager::ReleaseCollidable( Collidable* c )
{
	c->SetDestroyed( true );

	for ( int i = 0; i < m_vCollidables.size(); i++ )
	{
		if ( m_vCollidables[ i ]->IsDestroyed() )
		{
			delete m_vCollidables[ i ];
			m_vCollidables.erase(m_vCollidables.begin() + i);
		}
	}
}

void CollisionManager::ReleaseAllCollidables()
{
	for ( int i = 0; i < m_vCollidables.size(); i++ )
	{		
		delete m_vCollidables[ i ];
	}
	m_vCollidables.clear();
}

bool CollisionManager::IsIntersectionBetweenAsteroids( const Vector3f& testPos, float radius, const std::vector< Vector3f >& positions ) const
{
	for ( const Vector3f& pos : positions )
	{
		if ( length( pos - testPos ) < radius * 2 ) return true;
	}

	for ( StaticObject obj : m_vStaticObjects )
	{
		float distance = length( testPos - obj.pos );
		float radialSum = radius + obj.radius;
		if ( distance < radialSum ) return true;
	}
	return false;
}

bool CollisionManager::IsObjectIntersectingObject( const Vector3f& testPos, float radius, const std::vector< Vector3f >& positions) const
{
	for (int i = 0; i < m_vCollidables.size() - 1; i++)
	{
		Collidable* c = m_vCollidables[ i ];
		if ( c->IsIgnored() ) continue;

		float distance = length( c->GetBoundSphere().position - testPos );
		float radialSum = c->GetBoundSphere().radius + radius;

		if ( distance < radialSum ) return true;
		
	}
	return false;
}

void CollisionManager::CreateDebugSphere( Collidable* col )
{
	GameObjectManager::Instance().CreateGameObject< ExampleGameObject >(col);
}

bool CollisionManager::WillCollisionOccur( Collidable* col, const Vector3f& offset )
{
	for (int i = 0; i < m_vCollidables.size() - 1; i++)
	{
		Collidable* c = m_vCollidables[ i ];

		Vector3f posObj = col->GetBoundSphere().position + offset;
		float distance = length( posObj - c->GetBoundSphere().position );
		float radialSum = c->GetBoundSphere().radius + col->GetBoundSphere().radius;

		if ( distance < radialSum ) return true;
	}

	return false;

}

void CollisionManager::AddStaticObject( const Vector3f& pos, float radius )
{ 
	StaticObject s( pos, radius );
	m_vStaticObjects.push_back( s ); 
}