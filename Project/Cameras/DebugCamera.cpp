#include "GameMath.h"
#include "Types.h"
#include "DebugCamera.h"



DebugCamera::DebugCamera()
{
	Graphics::SurfaceSize surfaceSize = Graphics::GetDisplaySurfaceSize();
	f32 aspect( (f32)surfaceSize.m_width / (f32)surfaceSize.m_height );

	m_viewMatrix = MatrixLookatRH( m_eyePosition, m_eyePosition + m_vForward, m_vUp );
	m_projMatrix = MatrixPerspectiveProjectRH( m_fFOV, aspect, m_fNearClip, m_fFarClip );
}

void DebugCamera::Update()
{
	constexpr f32 kfRotateSpeed = 0.005f;
	constexpr f32 kfSpeed = 5.0f;

	f32 dT = System::GetDeltaTime();
	Input::MouseState mouseState = Input::GetMouseState();

	if ( mouseState.m_rightButton )
	{
		if ( !m_bIsCaptured )
		{
			Input::CaptureMouse( true );
			m_bIsCaptured = true;
		}

		m_fAzimuth += mouseState.m_deltaX * -kfRotateSpeed;
		m_fElevation += mouseState.m_deltaY * kfRotateSpeed;

		if ( m_fAzimuth > kfPi ) { m_fAzimuth -= kfTwoPi; }
		if ( m_fAzimuth < -kfPi ) { m_fAzimuth += kfTwoPi; }

		if ( m_fElevation > kfHalfPi ) { m_fElevation = kfHalfPi; }
		if ( m_fElevation < -kfHalfPi ) { m_fElevation = -kfHalfPi; }
	}
	else if ( m_bIsCaptured )
	{
		Input::CaptureMouse( false );
		m_bIsCaptured = false;
	}

	f32 cosAz = cos( m_fAzimuth );
	f32 sinAz = sin( m_fAzimuth );
	f32 cosEl = cos( m_fElevation );
	f32 sinEl = sin( m_fElevation );

	m_vRight = Vector3f( cosAz, 0.f, -sinAz );
	m_vUp = Vector3f( sinAz * sinEl, cosEl, cosAz * sinEl );
	m_vForward = Vector3f( sinAz * cosEl, -sinEl, cosAz * cosEl );

	Vector3f vDelta( 0.f, 0.f, 0.f );

	if ( Input::IsKeyDown('W') )
	{
		vDelta.z += 1;
	}
	if ( Input::IsKeyDown('S') )
	{
		vDelta.z += -1;
	}
	if ( Input::IsKeyDown('A') )
	{
		vDelta.x += 1;
	}
	if ( Input::IsKeyDown('D') )
	{
		vDelta.x += -1;
	}
	if ( Input::IsKeyDown('E') )
	{
		vDelta.y += -1;
	}
	if ( Input::IsKeyDown('Q') )
	{
		vDelta.y += 1;
	}

	m_eyePosition += m_vForward * vDelta.z * dT * kfSpeed;
	m_eyePosition += m_vRight * vDelta.x * dT * kfSpeed;
	m_eyePosition += m_vUp * vDelta.y * dT * kfSpeed;

	Graphics::SurfaceSize surfaceSize = Graphics::GetDisplaySurfaceSize();

	f32 aspect( (f32)surfaceSize.m_width / (f32)surfaceSize.m_height );

	m_viewMatrix = MatrixLookatRH( m_eyePosition, m_eyePosition + m_vForward, m_vUp );
	m_projMatrix = MatrixPerspectiveProjectRH( m_fFOV, aspect, m_fNearClip, m_fFarClip );
}

void DebugCamera::SetPosition( const Vector3f& fPosition, f32 fAzimuth, f32 fElevation )
{
	m_eyePosition = fPosition;
	m_fAzimuth = fAzimuth;
	m_fElevation = fElevation;
}

void DebugCamera::SetFOV( f32 fFOV, f32 fNearClip, f32 fFarClip )
{
	m_fFOV = fFOV;
	m_fNearClip = fNearClip;
	m_fFarClip = fFarClip;
}
