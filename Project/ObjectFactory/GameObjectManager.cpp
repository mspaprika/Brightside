#include "GameMath.h"
#include "Types.h"
#include "Collisions/CollisionManager.h"
#include "VertexData.h"
#include "Objects/Asteroid.h"
#include "Objects/Debris.h"
#include "Objects/Volcano.h"
#include "GameObjectManager.h"
#include "RenderContext.h"

///////////////////////////////////////////////////////////////////////////
//	File		: GameObjectManager.cpp
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////


PLAY_SINGLETON_IMPL( GameObjectManager );

GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{
	DestroyAllObjects();
	
}

void GameObjectManager::Update( float dT )
{
	for ( auto obj : m_pGameObjects )
	{
		if ( !obj->IsObjDead() )
			obj->Update( dT );
	}
}

void GameObjectManager::PostUpdate()
{
	m_pGameObjects.erase( std::remove_if( m_pGameObjects.begin(), m_pGameObjects.end(), 
		[]( GameObject* obj ) {
		if ( obj->IsObjDead() )
		{
			delete obj;
			return true;
		}
		return false; }), m_pGameObjects.end() );

	m_pGameObjects.insert( m_pGameObjects.end(), m_pCreatedGameObjects.begin(), m_pCreatedGameObjects.end() );
	m_pCreatedGameObjects.clear();
}

void GameObjectManager::Render( RenderContext& ctx ) const
{
	for ( const auto obj : m_pGameObjects )
	{
		obj->Render( ctx );
	}
}

GameObject* GameObjectManager::GetObjectByType( GameObjectType type )
{
	for ( GameObject* obj : m_pCreatedGameObjects )
	{
		if (obj->GetType() == type)
		{		
			return obj;
		}
	}
	for ( GameObject* obj: m_pGameObjects )
	{
		if ( obj->GetType() == type )
		{
			return obj;
		}
	}
	return nullptr;
}

void GameObjectManager::DestroyAllObjects()
{
	for ( GameObject* obj : m_pCreatedGameObjects )
	{
		delete obj;
	}
	m_pCreatedGameObjects.clear();

	for ( GameObject* obj : m_pGameObjects )
	{
		delete obj;
	}
	m_pGameObjects.clear();
	m_volcanoPositions.clear();
}

void GameObjectManager::CreateAsteroids( AsteroidType type, int count, Vector3f position, float dist )
{
	/*
	* other way to bind std::function
	std::vector <Vector3f> asteroidPositions = GameObjectManager::Instance().GenerateAsteroidPositions(count, AsteroidType::TYPE_LARGE,
		std::bind(&CollisionManager::IsIntersection, &CollisionManager::Instance(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
		);*/

	std::vector< Vector3f > asteroidPositions;
	asteroidPositions.reserve( count );

	if ( type != AsteroidType::TYPE_LARGE )
	{
		asteroidPositions = GenerateAsteroidPositions( count, type,
			[]( const Vector3f& testPos, float radius, const std::vector< Vector3f >& positions )
			{ return CollisionManager::Instance().IsObjectIntersectingObject( testPos, radius, positions ); }, position, dist );}
	else 
	{
		asteroidPositions = GenerateAsteroidPositions( count, type,
			[]( const Vector3f& testPos, float radius, const std::vector< Vector3f >& positions )
			{ return CollisionManager::Instance().IsIntersectionBetweenAsteroids( testPos, radius, positions ); });
	}

	for (int i = 0; i < asteroidPositions.size(); i++)
	{
		Matrix4x4f asteroidWorld = GenerateWorld( asteroidPositions[ i ] );
		GameObjectManager::Instance().CreateGameObject< Asteroid >(asteroidWorld, type);
	}
}

std::vector< Vector3f > GameObjectManager::GenerateAsteroidPositions( int count, AsteroidType type, CollisionCheckFunction function, Vector3f position, float separationDist )
{
	bool isLarge = separationDist == 0.0f; // if default argument is not passed it means we're making large asteroids
	std::vector< Vector3f > positions{};
	positions.reserve( count );

	for (int i = 0; i < count; i++)
	{
		Vector3f randomPos = RandVector3f();
		// for large asteroid position random pos is used, for smaller ones it's calculated with origin + offset * some distance 
		Vector3f pos = isLarge ? normalize( randomPos ) : normalize( position + randomPos * separationDist );
		// get player pos to check if new asteroid is not above the player
		Vector3f playerPos = normalize( GetPlayer()->GetPosition() );
		float radius = g_vAsteroidRadius[ static_cast< int >(type) ];

		// 2 checks for new position: is it above player; is it intersecting asteroids/colliding other objects; 	
		while (( isLarge && dot( pos, playerPos ) > 0.2f ) || function( pos, radius, positions ))
		{
			randomPos = RandVector3f();
			pos = isLarge ? normalize( randomPos ) : normalize( position + randomPos * separationDist );
		}
		
		positions.push_back( pos );
	}
	return positions;
}

void GameObjectManager::CreateDebris(int count, AsteroidType type, Vector3f position, float separationDist)
{
	for ( int i = 0; i < count; i++ )
	{
		Vector3f pos = normalize( position + RandVector3f() * separationDist );
		Matrix4x4f debrisWorld = GenerateWorld( pos );
		GameObjectManager::Instance().CreateGameObject< Debris >(debrisWorld, type);
	}
}

Matrix4x4f GameObjectManager::GenerateWorld( Vector3f position )
{
	Matrix4x4f ret = MatrixTranslate( position );

	Vector3f newUp = normalize( position );
	Vector3f newRight = cross( newUp, RandVector3f() );
	Vector3f newForward = normalize( cross(newRight, newUp) );
	newRight = normalize( cross( newUp, newForward) );
	ret.m_column[ 0 ] = { newRight, 0 };
	ret.m_column[ 1 ] = { normalize( position ), 0 };
	ret.m_column[ 2 ] = { newForward, 0 };

	return ret;
}

void GameObjectManager::CreateVolcanos( VolcanoType type, int count )
{
	Vector3f playerPos = normalize( GetPlayer()->GetPosition() );
	m_volcanoPositions.reserve( count );

	for ( int i = 0; i < count; i++ )
	{
		Vector3f volcanoPos = normalize(vertexData[ RandomNumberRange(0, 18) ]);

		while (dot( playerPos, normalize(volcanoPos) ) > 0.5f || AreAnyObjectsFacingSameDirection( m_volcanoPositions, volcanoPos, 0.5f ))
		{
			volcanoPos = normalize(vertexData[ RandomNumberRange(0, 18) ]);
		}

		CollisionManager::Instance().AddStaticObject( volcanoPos, 0.1f );

		Matrix4x4f world = GenerateWorld( volcanoPos);
		GameObjectManager::Instance().CreateGameObject< Volcano >(type, world);
		m_volcanoPositions.push_back( volcanoPos );
	}
}

std::vector< GameObject* > GameObjectManager::CollectGameObjectsByType( GameObjectType type )
{
	std::vector< GameObject* > v_objList{};
	v_objList.reserve( 10 );

	for ( GameObject* obj : m_pCreatedGameObjects )
	{
		if ( obj->GetType() == type ) v_objList.push_back( obj );
	}

	for ( GameObject* obj : m_pGameObjects )
	{
		if ( obj->GetType() == type ) v_objList.push_back( obj );
	}
	return v_objList;
}



	


