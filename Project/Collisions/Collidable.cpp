#include "GameMath.h"
#include "Types.h"
#include "ObjectFactory/GameObject.h"
#include "Collidable.h"


Collidable::Collidable( GameObject* obj, const Vector3f pos, float radius, bool ignored )
{
	m_boundSphere.position = pos;
	m_boundSphere.radius = radius;
	m_pObj = obj;
	m_ignored = ignored;
}

Collidable::~Collidable()
{

}




