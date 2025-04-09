#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "PowerUp.h"
#include "Collisions/CollisionManager.h"
#include "ObjectFactory/GameObjectManager.h"
#include "Particles/AsteroidExplosion.h"
#include "Asteroid.h"
#include "RenderContext.h"



Asteroid::Asteroid( const Matrix4x4f& world, AsteroidType type )
{
	m_materialId = AssetManager::Instance().GetAsset( ConvertType(type) ).mat;
	m_meshId = AssetManager::Instance().GetAsset( ConvertType(type) ).mesh;
	m_transform = world;
	SetPosition( GetPosition() + GetUp() * kOffsetMultiplier );
	m_drawTransform = MatrixIdentity4x4f();
	m_speed = 0.3f;
	m_rotAngle = RandomFloatRange( 0.5f, 1.f );
	m_asteroidType = type;

	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), g_vAsteroidRadius[ static_cast< int >(type) ] );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCol );
}

Asteroid::~Asteroid()
{
	CollisionManager::Instance().ReleaseCollidable( m_pCol );
}

void Asteroid::Update( float dT )
{
	ManageActivity( dT, 1.f );
	OrbitalRotation( dT );

	// update drawable matrix
	SetDrawPosition( GetPosition() );
	m_drawTransform = m_drawTransform * MatrixRotationZ( m_rotAngle * dT );

	m_pCol->UpdateBoundPosition( GetPosition() );
}

AsteroidType Asteroid::GetNewType() const 
{
	if ( m_asteroidType == AsteroidType::TYPE_LARGE ) return AsteroidType::TYPE_MEDIUM;
	else if ( m_asteroidType == AsteroidType::TYPE_MEDIUM ) return AsteroidType::TYPE_SMALL;
	else return m_asteroidType;
}

void Asteroid::ResolveCollision( GameObject* obj, float overlap )
{
	CollidableObject* colObj = ( CollidableObject* )obj;

	switch ( obj->GetType() )
	{
		case GameObjectType::TYPE_ASTEROID:
		case GameObjectType::TYPE_VOLCANO:
		case GameObjectType::TYPE_ENEMY:
			BounceOff( obj, overlap );
			break;

		case GameObjectType::TYPE_LASER:
			if ( !IsObjDead() && m_active && colObj->IsActive() ) 
			{
				Explode();
				CreateRandomPowerUp();
				GameObjectManager::Instance().GetPlayer()->AddScore( 100 );
			}
			break;

		case GameObjectType::TYPE_PLAYER:
			Player* p = ( Player* )obj;
			if ( p->IsBlastActive() ) Explode();
			else BounceOff( obj, overlap );
			break;
	}
}

void Asteroid::CreateRandomPowerUp() const
{
	int rand = RandomNumberRange( 1, 3 );

	if (RandomNumberRange( 1, 3 ) == 2)
	{
		switch( rand )
		{
			case 1:
				GameObjectManager::Instance().CreateGameObject< PowerUp >(m_transform, PowerUpType::TYPE_HEALTH);			
				break;

			case 2:
				GameObjectManager::Instance().CreateGameObject< PowerUp >(m_transform, PowerUpType::TYPE_SHIELD);
				break;

			case 3:
				GameObjectManager::Instance().CreateGameObject< PowerUp >(m_transform, PowerUpType::TYPE_SPEED);
				break;

			default:
				break;
		}
	}
}

void Asteroid::Explode()
{
	DestroySelf();
	GameObjectManager::Instance().CreateGameObject< AsteroidExplosion >(ParticleType::TYPE_PIXEL, GetPosition(), 500, 1.f);

	if ( m_asteroidType != AsteroidType::TYPE_SMALL )
		GameObjectManager::Instance().CreateAsteroids( GetNewType(), 3, GetPosition(), m_separatioDistance );
}

AssetType Asteroid::ConvertType( AsteroidType type )
{
	if ( type == AsteroidType::TYPE_SMALL ) return AssetType::TYPE_ASTEROID_S;
	else if ( type == AsteroidType::TYPE_MEDIUM ) return AssetType::TYPE_ASTEROID_M;
	else return AssetType::TYPE_ASTEROID_L;
}

void Asteroid::Render( RenderContext& ctx ) const 
{
	if( ctx.GetRenderPassType() != RenderPassType::kDebugPass )
	{
		MeshRenderCommand cmd;
		cmd.meshId = m_meshId;
		cmd.materialId = m_materialId;
		cmd.worldMatrix = m_drawTransform;
		ctx.RenderMesh( cmd );
	}
	else // collect commands only if right pass is being rendered
	{
		AxisRenderCommand cmd;
		cmd.worldMatrix = m_transform;
		ctx.RenderAxis( cmd );
	}
}

void Asteroid::BounceOff( GameObject* obj, float overlap )
{
	Vector3f bounceDirection = normalize( GetPosition() - obj->GetPosition() );

	// cross product for RIGHT axis is flipped
	Vector3f newForward = bounceDirection;
	Vector3f newPos = GetPosition() + newForward * overlap * 2.f;
	Vector3f newUp = normalize( newPos );
	Vector3f newRight = cross( newUp, newForward );

	SetRight( normalize( newRight ) );
	SetUp( normalize(newPos) );
	SetForward( newForward );
	SetPosition( normalize(newPos) );
}
