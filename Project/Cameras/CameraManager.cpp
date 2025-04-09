///////////////////////////////////////////////////////////////////////////
//	File		: AIHarness/Camera.cpp
//	Platform	: All
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////
#include "GameMath.h"
#include "Types.h"
#include "CameraManager.h"




// Camera Manager
PLAY_SINGLETON_IMPL(CameraManager);
CameraManager::CameraManager()
{

}

CameraManager::~CameraManager()
{
	for (auto pCamera : m_cameras)
	{
		delete pCamera;
	}
}

u32 CameraManager::RegisterCamera( Camera* pCamera )
{
	u32 id = (u32)m_cameras.size();
	m_cameras.push_back( pCamera );
	return id;
}

void CameraManager::SetActiveCamera( u32 id )
{
	PLAY_ASSERT( m_activeCameraId < m_cameras.size() );
	m_activeCameraId = id;
}

void CameraManager::Update()
{
	GetActiveCamera().Update();
}

void CameraManager::ApplyActiveCameraMatrices()
{
	Graphics::SetViewMatrix( GetActiveCamera().GetView() );
	Graphics::SetProjectionMatrix( GetActiveCamera().GetProject() );
}

Camera& CameraManager::GetActiveCamera()
{
	PLAY_ASSERT(m_activeCameraId < m_cameras.size() && m_cameras[ m_activeCameraId ]);
	return *m_cameras[ m_activeCameraId ];
}

void CameraManager::DestroyPlayerCamera()
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		if ( m_cameras[ i ]->IsPlayerCam() )
		{
			delete m_cameras[ i ]; // release the memory of player's camera
			m_cameras.erase( m_cameras.begin() + i ); // erase player's camera pointer from the list
			m_activeCameraId = m_debugCamID; // set debug camera as active
			return;
		}
	}
}

