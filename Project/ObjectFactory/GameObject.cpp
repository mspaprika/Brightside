#include "GameMath.h"
#include "Types.h"
#include "GameObject.h"


GameObject::GameObject() : m_bObjBeingDestroyed( false )
{
}

GameObject::~GameObject()
{

}

void GameObject::OrbitalRotation( float dT )
{
	// all axis are correct here and not flipped
	Vector3f newPosition = GetPosition() + (GetForward() * m_speed * dT);
	Vector3f newUp = normalize( newPosition );
	Vector3f newForward = cross( GetRight(), newUp );
	Vector3f newRight = cross( newUp, newForward );

	SetRight( normalize(newRight) );
	SetUp( newUp );
	SetForward( normalize(newForward) );
	SetPosition( normalize(newPosition) + newUp * m_offsetMult );
}