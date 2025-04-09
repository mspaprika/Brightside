#pragma once

#include "GameObject.h"
#include "Objects/Player.h"
#include "Objects/Enemy.h"
#include "Particles/ExampleParticleFX.h"
#include "Collisions/CollisionManager.h"

///////////////////////////////////////////////////////////////////////////
//	File		: GameObjectManager.h
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

class RenderContext;

template <typename T>
concept GameObjectable = std::is_base_of<GameObject, T>::value;

class GameObjectManager
{
	GameObjectManager();
	~GameObjectManager();

	PLAY_SINGLETON_INTERFACE(GameObjectManager);

public:
	using CollisionCheckFunction = std::function< bool( const Vector3f&, float, const std::vector< Vector3f >& ) >;

	void Update( float dT );
	void PostUpdate();
	void Render( RenderContext& ctx ) const;
	void DestroyAllObjects();

	GameObject* GetObjectByType( GameObjectType type );

	Player* GetPlayer() { return ( Player* )GetObjectByType( GameObjectType::TYPE_PLAYER ); }
	Enemy* GetEnemy() { return ( Enemy* )GetObjectByType( GameObjectType::TYPE_ENEMY ); }

	void CreateAsteroids( AsteroidType type, int count, Vector3f pos = { 0.0f, 0.0f, 0.0f }, float dist = 0.0f );
	std::vector< Vector3f > GenerateAsteroidPositions( int count, AsteroidType type, CollisionCheckFunction function, Vector3f pos = { 0.0f, 0.0f, 0.0f }, float separationDist = 0.0f );
	
	Matrix4x4f GenerateWorld( Vector3f position );
	void CreateVolcanos( VolcanoType type, int count );
	void CreateDebris( int count, AsteroidType type, Vector3f position, float separationDist );
	
	
	std::vector< GameObject* > CollectGameObjectsByType( GameObjectType type );
	
	template<GameObjectable T, typename... Args> 
	T* CreateGameObject( Args... args );

private:
	std::vector< GameObject* > m_pCreatedGameObjects;
	std::vector< GameObject* > m_pGameObjects;
	std::vector< Vector3f > m_volcanoPositions;
};

template< GameObjectable T, typename... Args >
T* GameObjectManager::CreateGameObject( Args... args )
{
	T* newObj = new T( args ... );
	m_pCreatedGameObjects.push_back( newObj );
	
	return newObj;
}