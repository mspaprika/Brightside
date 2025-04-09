#include "GameMath.h"
#include "Types.h"
#include "Objects/Player.h"
#include "PlayerCamera.h"
#include <random>
#include <chrono>



PlayerCamera::PlayerCamera()
{
	m_playerCam = true;
	m_generator.seed(static_cast< unsigned long >(std::chrono::system_clock::now().time_since_epoch().count()));
}

void PlayerCamera::Update()
{
	if (m_shakeActive) ApplyShakeEffect( System::GetDeltaTime() );
	else
	{
		// camera pos = player pos + player UP * distance
		Vector3f m_camPosition = m_pPlayer->GetPosition() + m_pPlayer->GetUp() * m_offset;

		Matrix4x4f cameraWorld = MatrixIdentity4x4f();
		// camera position = player pos + player up * distance
		cameraWorld.m_column[ 3 ] = { m_camPosition, 1 };
		// camera forward == player's up
		cameraWorld.m_column[ 2 ] = { m_pPlayer->GetUp() , 0 };
		// camera up == player's velocity which is the direction player last time moved
		cameraWorld.m_column[ 1 ] = { normalize(m_pPlayer->GetVelocity()), 0 };
		//  camera right = cross(forward, up)
		Vector3f newRight = cross( cameraWorld.m_column[ 1 ].xyz() , cameraWorld.m_column[ 2 ].xyz() );
		cameraWorld.m_column[ 0 ] = { normalize(newRight), 0 };

		// Update internal and inverse matrices
		m_pPlayer->UpdateCameraInverseMtx( cameraWorld, InversePerspectiveProjectRH( m_projMatrix ) );
		m_viewMatrix = AffineInverse( cameraWorld );
	}
}

void PlayerCamera::SetPlayer( Player* p )
{
	m_pPlayer = p;

	Graphics::SurfaceSize surfaceSize = Graphics::GetDisplaySurfaceSize();
	f32 aspect( (f32)surfaceSize.m_width / (f32)surfaceSize.m_height );

	Matrix4x4f cameraWorld = MatrixIdentity4x4f();

	cameraWorld.m_column[ 3 ] = { p->GetPosition() + p->GetUp() * m_offset, 1 };
	cameraWorld.m_column[ 2 ] = { -p->GetUp(), p->GetWorldMtx().m_column[ 1 ].w };
	cameraWorld.m_column[ 1 ] = { p->GetForward(), 0 };
	cameraWorld.m_column[ 0 ] = p->GetWorldMtx().m_column[ 0 ];

	m_viewMatrix = AffineInverse( cameraWorld );
	m_projMatrix = MatrixPerspectiveProjectRH( m_fFOV, aspect, m_fNearClip, m_fFarClip );
}

void PlayerCamera::ApplyShakeEffect(float dT)
{
	if (m_shakeTimer < m_shakeDuration) 
	{
		Vector3f shakeOffset = Vector3f(m_distribution( m_generator ), m_distribution( m_generator ), m_distribution( m_generator ));
		m_viewMatrix =  MatrixTranslate( shakeOffset ) * m_viewMatrix;
		m_shakeTimer += dT;
		
	}
	else {
		// Reset shake parameters
		m_shakeIntensity = 0.0f;
		m_shakeDuration = 0.0f;
		m_shakeTimer = 0.0f;
		m_shakeActive = false;
	}
}

void PlayerCamera::Shake(float intensity, float duration)
{
	m_shakeIntensity = intensity;
	m_shakeDuration = duration;
	m_shakeActive = true;
	m_shakeTimer = 0.0f;
	m_distribution = std::uniform_real_distribution<float>(-intensity, intensity);
}

