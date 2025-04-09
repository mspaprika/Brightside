#pragma once
///////////////////////////////////////////////////////////////////////////
//	File		: GameObject.h
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////
class RenderContext;
class Collidable;


class GameObject
{
private:
	GameObject( const GameObject& ) = delete;
	GameObject& operator= (const GameObject&) = delete;
protected:
	GameObject();
public:
	virtual ~GameObject();
	virtual void Update( float dT ) = 0;
	virtual void Render( RenderContext& ctx ) const = 0;
	virtual GameObjectType GetType() const = 0;

	Vector3f GetRight() const { return m_transform.m_column[ 0 ].xyz(); }
	Vector3f GetUp() const { return m_transform.m_column[ 1 ].xyz(); }
	Vector3f GetForward() const { return m_transform.m_column[ 2 ].xyz(); }
	Vector3f GetPosition() const { return m_transform.m_column[ 3 ].xyz(); }

	void SetRight(Vector3f right) { m_transform.m_column[ 0 ] = { right, 0 }; }
	void SetUp( Vector3f up ) { m_transform.m_column[ 1 ] = { up, 0 }; }
	void SetForward( Vector3f forward ) { m_transform.m_column[ 2 ] = { forward, 0 }; }
	void SetPosition( Vector3f pos ) { m_transform.m_column[ 3 ] = { pos , 1 }; }

	inline void DestroySelf() { m_bObjBeingDestroyed = true; } // Call this when you want to destroy an object, then set any pointers to this to nullptr.
	inline bool IsObjDead() const { return m_bObjBeingDestroyed; } // Use this to check if you have a dead object.
	void OrbitalRotation( float dT );

	Matrix4x4f m_transform;
	Graphics::MeshId m_meshId;
	Graphics::MaterialId m_materialId;
	bool m_bObjBeingDestroyed;
	float m_offsetMult{ kOffsetMultiplier };
	float m_speed{ 1.0f };

	//bool m_debugMode{ true };
	bool m_debugMode{ false };
};