#include "GameMath.h"
#include "Types.h"
#include "Cameras/PlayerCamera.h"
#include "ShakeManager.h"


PLAY_SINGLETON_IMPL( ShakeManager );

ShakeManager::ShakeManager()
{

}

ShakeManager::~ShakeManager()
{

}

void ShakeManager::ShakePlayerCamera( float intensity, float duration )
{
	m_pPlayerCam->Shake( intensity, duration );
}

bool ShakeManager::IsActive()
{
	return m_pPlayerCam->IsActive();
}