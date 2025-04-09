#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Collisions/CollisionManager.h"
#include "ObjectFactory/GameObjectManager.h"
#include "Utilities/LightManager.h"
#include "Utilities/SoundManager.h"
#include "Missile.h"
#include "Enemy.h"
#include "RenderContext.h"


Enemy::Enemy( u32 shootInterval, float speed )
{
	m_materialId = AssetManager::Instance().GetAsset( AssetType::TYPE_ENEMY ).mat;
	m_meshId = AssetManager::Instance().GetAsset( AssetType::TYPE_ENEMY ).mesh;
	m_transform = MatrixTranslate( Vector3f(0.0f, kOffsetMultiplier * 2.f, 0.0f) );
	m_speed = speed;
	m_shootInterval = shootInterval;
	m_offsetMult = 1.3f;
	m_drawTransform = MatrixIdentity4x4f();
	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), 0.4f );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCol );
}

Enemy::~Enemy()
{
	CollisionManager::Instance().ReleaseCollidable( m_pCol );
}

void Enemy::Update( float dT )
{ 
	ManageActivity( dT, 0.1f );
	if ( m_active ) UpdateActiveMode( dT );

	UpdateAttackMode();
	UpdateDrawableMtx();
	UpdateSpotlight();

	m_pCol->UpdateBoundPosition( GetPosition() );
}

void Enemy::UpdateSpotlight()
{
	Vector3f newDir = RotateAroundAxis( -GetDrawUp() , GetDrawForward(), 0.8f );
	Vector3f parentedDir = Transform(m_drawTransform, Vector4f( newDir, 0) ).xyz();

	LightManager::Instance().SetSpotlightPosition( LightType::ENEMY_SPOT, GetPosition() + GetDrawRight() * 0.9f );
	LightManager::Instance().SetSpotlightDirection( LightType::ENEMY_SPOT, -GetUp());

	if ( !m_gameActive ) LightManager::Instance().TurnOffLight( LightType::ENEMY_SPOT );
}

void Enemy::UpdateDrawableMtx()
{
	// update draw transform with base transform rotated by a factor of elapsed time
	m_drawTransform = m_transform * MatrixRotationY( 0.8f * (f32)System::GetElapsedTime() );
	SetDrawPosition( GetPosition() );
}

void Enemy::UpdateActiveMode( float dT )
{
	 OrbitalRotation( dT );

	if (GameObjectManager::Instance().GetPlayer()->IsBlastActive())
	{
		m_activityTimer = 0.0f;
		m_active = false;
	}
}

void Enemy::UpdateAttackMode()
{
	Vector3f playerPos = GameObjectManager::Instance().GetPlayer()->GetPosition();
	float distance = length( playerPos - GetPosition() );

	(distance < m_attackDist) && m_gameActive ? SetAttackMode() : SetIdleMode();
	
	if ( m_attackMode )
	{
		if ( RandomNumberRange(1, m_shootInterval) == 1 )
		{
			Matrix4x4f missileWorld = m_drawTransform;
			Vector3f offsetToGround = normalize( playerPos ) - GetPosition();
			Vector3f transformed = Transform( MatrixRotationZ(0.1f), Vector4f(-GetUp(), 0.0f) ).xyz();
			missileWorld.m_column[ 2 ] = { normalize( transformed ), 0 };

			GameObjectManager::Instance().CreateGameObject< Missile >(missileWorld);
			SoundManager::Instance().PlaySound( SoundType::HUM );
		}
	}
}

void Enemy::SetAttackMode()
{
	m_attackMode = true;
	LightManager::Instance().SetSpotlightColour( LightType::ENEMY_SPOT, Colour::SumoRed );
}

void Enemy::SetIdleMode()
{
	m_attackMode = false;
	LightManager::Instance().SetSpotlightDefaults( LightType::ENEMY_SPOT );
}

void Enemy::ResolveCollision( GameObject* obj, float overlap )
{
	CollidableObject* colObj = ( CollidableObject* )obj;

	switch ( obj->GetType() )
	{
		case GameObjectType::TYPE_ASTEROID:
		case GameObjectType::TYPE_VOLCANO:
			BounceOff( obj, overlap );
			break;

		default:
			break;
	}
}

void Enemy::BounceOff( GameObject* obj, float overlap )
{
	Vector3f bounceDirection = normalize( GetPosition() - obj->GetPosition() );

	Vector3f newForward = bounceDirection;
	Vector3f newPos = GetPosition() + newForward * overlap * 2.f;
	Vector3f newUp = normalize( newPos );
	Vector3f newRight = cross( newUp, newForward);

	SetRight( normalize(newRight) );
	SetUp( normalize(newPos) );
	SetForward( newForward );
	SetPosition( normalize(newPos) );
}

void Enemy::Render( RenderContext& ctx ) const
{
	if ( ctx.GetRenderPassType() != RenderPassType::kDebugPass )
	{
		MeshRenderCommand cmd;
		cmd.meshId = m_meshId;
		cmd.materialId = m_materialId;
		cmd.worldMatrix = m_drawTransform;
		ctx.RenderMesh( cmd );
	}
	else
	{
		AxisRenderCommand cmd;
		//cmd.worldMatrix = m_drawTranform;
		cmd.worldMatrix = m_transform;
		ctx.RenderAxis( cmd );
	}
}