///////////////////////////////////////////////////////////////////////////
//	File		: AIHarness/Camera.h
//	Platform	: All
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

#pragma once

class PlayerCamera;

// Base class for cameras
class Camera
{
public:
	Camera() {}
	virtual ~Camera() {}

	// Update should be overloaded to update internal matrices.
	virtual void Update() = 0;

	const Matrix4x4f& GetView() const { return m_viewMatrix; }
	const Matrix4x4f& GetProject() const { return m_projMatrix; }

	bool IsPlayerCam() { return m_playerCam; }


protected:
	f32 m_fAzimuth = 0.f;
	f32 m_fElevation = kfPi / 16;
	f32 m_fNearClip = 0.1f;
	f32 m_fFarClip = 100.f;
	f32 m_fFOV = kfPi / 4.f;

	Matrix4x4f m_viewMatrix;
	Matrix4x4f m_projMatrix;
	bool m_playerCam{ false };
};

// Singleton class for managing cameras.
class CameraManager
{
	PLAY_SINGLETON_INTERFACE(CameraManager);
	CameraManager();
	~CameraManager();
public:
	u32 RegisterCamera(Camera* pCamera);
	void ApplyActiveCameraMatrices();
	void DestroyPlayerCamera();
	// Updates the active camera
	void Update();

	void SetActiveCamera(u32 id);
	u32 GetDebugCamID() const { return m_debugCamID; }
	void SetDebugCamID(u32 id) { m_debugCamID = id; }

	u32 GetPlayerCamID() const { return m_playerCamID; }
	void SetPlayerCamID(u32 id) { m_playerCamID = id; }

	Camera& GetActiveCamera();
	PlayerCamera* GetPlayerCamera() { return (PlayerCamera*)m_cameras[m_playerCamID]; }
	
private:
	std::vector<Camera*> m_cameras;
	u32 m_activeCameraId = 1;
	u32 m_debugCamID{ 0 };
	u32 m_playerCamID{ 0 };
};


// -------------------------------------------------------------------------------

