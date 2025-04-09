#pragma once
#include "CameraManager.h"



class DebugCamera : public Camera
{
public:
	DebugCamera();
	void Update() override;
	void SetPosition( const Vector3f& fPosition, f32 fAzimuth, f32 fElevation );
	void SetFOV( f32 fFOV, f32 fNearClip, f32 fFarClip );

private:
	Vector3f m_eyePosition = Vector3f( 0.f, 1.f, -3.5f );
	Vector3f m_vForward = Vector3f( 0, 0, 1 );
	Vector3f m_vRight = Vector3f( 1, 0, 0 );
	Vector3f m_vUp = Vector3f( 0, 1, 0 );
	bool m_bIsCaptured = false;
};