#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Collisions/CollisionManager.h"
#include "ObjectFactory/GameObjectManager.h"
#include "Utilities/ShakeManager.h"
#include "Utilities/SoundManager.h"
#include "Particles/AsteroidExplosion.h"
#include "Missile.h"
#include "RenderContext.h"


Missile::Missile( const Matrix4x4f& world )
{
	m_materialId = AssetManager::Instance().GetAsset( AssetType::TYPE_MISSILE ).mat;
	m_meshId = AssetManager::Instance().GetAsset( AssetType::TYPE_MISSILE ).mesh;
	m_transform = world;
	m_speed = 2.0f;

	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), 0.05f, true );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCol );
}

Missile::~Missile()
{
	CollisionManager::Instance().ReleaseCollidable(m_pCol);
}

void Missile::Update( float dT )
{
	m_transform =  MatrixRotationZ( m_rotAngle * dT ) * m_transform;
	m_offsetMult *= 0.97f;

	if ( m_exploded ) 
	{
		DestroySelf(); // destroy missile in the next frame after it's explosion
		GameObjectManager::Instance().CreateDebris( 3, AsteroidType::TYPE_DEBRIS_S, GetPosition(), 1.5f );
		GameObjectManager::Instance().CreateDebris( 3, AsteroidType::TYPE_DEBRIS_M,  GetPosition(), 1.5f );
	}

	PlanetCollision();
	ManageActivity( dT, 0.1f );
	OrbitalRotation( dT );

	m_pCol->UpdateBoundPosition(GetPosition());
}

void Missile::PlanetCollision()
{
	float distance = length( GetPosition() );
	float radialSum = m_pCol->GetBoundSphere().radius + 1.4f;

	if ( distance < radialSum )
	{
		GameObjectManager::Instance().CreateGameObject< AsteroidExplosion >(ParticleType::TYPE_PIXEL, GetPosition(), 200, 1.2f, g_vParticleColours[ static_cast< int > (Color::L_YELLOW)] );
		ShakeManager::Instance().ShakePlayerCamera( 0.05f, 0.5f );
		m_pCol->UpdateSphereRadius( 1.5f ); // create large collidable sphere when missile explodes 
		m_exploded = true;	
		SoundManager::Instance().PlaySound( SoundType::EXPLOSION );
	}
}

void Missile::ResolveCollision( GameObject* obj, float overlap )
{
	switch (obj->GetType())
	{
		case GameObjectType::TYPE_ASTEROID:
		case GameObjectType::TYPE_PLAYER:
			DestroySelf();
			GameObjectManager::Instance().CreateGameObject< AsteroidExplosion >(ParticleType::TYPE_PIXEL, GetPosition(), 300, 1.0f);
			break;

		default:
			break;
	}
}

void Missile::Render( RenderContext& ctx ) const
{
	if ( ctx.GetRenderPassType() != RenderPassType::kDebugPass )
	{
		MeshRenderCommand cmd;
		cmd.meshId = m_meshId;
		cmd.materialId = m_materialId;
		cmd.worldMatrix = m_transform;
		ctx.RenderMesh( cmd );
	}
	else
	{
		AxisRenderCommand cmd;
		cmd.worldMatrix = m_transform;
		ctx.RenderAxis( cmd );
	}
}
