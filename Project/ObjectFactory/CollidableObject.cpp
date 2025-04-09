#include "GameMath.h"
#include "Types.h"
#include "CollidableObject.h"



void CollidableObject::ManageActivity( float dT, float delay )
{
	m_activityTimer += dT;
	if ( m_activityTimer > delay ) 
	{
		m_active = true;
	}
}