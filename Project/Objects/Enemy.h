#pragma once

#include "ObjectFactory/CollidableObject.h"


class RenderContext;

class Enemy : public CollidableObject
{
public:
	Enemy( u32 shootInterval, float speed );
	~Enemy() override;

	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	void ResolveCollision( GameObject* obj, float overlap ) override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_ENEMY; };
	void BounceOff( GameObject* obj, float overlap ) override;

	Vector3f GetDrawRight() const { return m_drawTransform.m_column[ 0 ].xyz(); }
	Vector3f GetDrawUp() const { return m_drawTransform.m_column[ 1 ].xyz(); }
	Vector3f GetDrawForward() const { return m_drawTransform.m_column[ 2 ].xyz(); }
	void SetDrawPosition( const Vector3f& pos ) { m_drawTransform.m_column[ 3 ] = { pos , 1 }; }

	void UpdateAttackMode();
	void UpdateSpotlight();
	void SetAttackMode();
	void UpdateActiveMode( float dT );
	void UpdateDrawableMtx();
	void SetIdleMode();
	void SetGameActive( bool isActive ) { m_gameActive = isActive; }

private:
	Matrix4x4f m_drawTransform;
	u32 m_shootInterval{ 0 };
	bool m_attackMode{ false };
	float m_attackDist{ 3.0f };
	bool m_gameActive{ true };
};