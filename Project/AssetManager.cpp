#include "GameMath.h"
#include "Types.h"
#include "Assets.h"
#include "AssetManager.h"



PLAY_SINGLETON_IMPL( AssetManager );

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::LoadGameAssets()
{
	LoadParticles();
	LoadDebugSphereMaterial();

	for ( int i = 0; i < static_cast< int >(AssetType::TOTAL_ASSETS); i++ )
	{
		MeshId mesh = Graphics::CreateMeshFromAssetFile(g_vAssetMeshPaths[ i ]);
		MaterialId material;

		if ( i != static_cast< int >(AssetType::TYPE_LASER) ) 
		{
			PbrDesc matDesc;
			{
				matDesc.m_bEnableLighting = true;
				matDesc.m_bEnableShadows = true;
				matDesc.m_constants.albedoColour = ( 1.f, 1.f, 1.f, 1.f );
				matDesc.m_constants.metalness = 1.0f; // Multiplier for texture based metal/roughness
				matDesc.m_constants.roughness = 1.0f;
				matDesc.m_lightCount = kNumLights;

				matDesc.m_texture[ 0 ] = Graphics::CreateTextureFromFile(g_vAssetColourPaths[ i ]);
				matDesc.m_texture[ 1 ] = Graphics::CreateTextureFromFile(g_vAssetNormalPaths[ i ]);
				matDesc.m_texture[ 2 ] = Graphics::CreateTextureFromFile(g_vAssetORMPaths[ i ]);
				matDesc.m_sampler[ 0 ] = matDesc.m_sampler[ 1 ] = matDesc.m_sampler[ 2 ] = Graphics::CreateLinearSampler();
			}
			material = Resources::CreateAsset< Material >(matDesc);
		}
		else
		{
			ComplexDesc matDesc;
			{
				matDesc.SetupFromHLSLFile( "SelfIllumination", g_vShaderPaths[ static_cast< int >(ShaderType::SELF_ILLUMINATE) ]);
				matDesc.m_state.m_cullMode = Graphics::CullMode::BACK;
				matDesc.m_state.m_fillMode = Graphics::FillMode::SOLID;
			}
			material = Resources::CreateAsset< Material >(matDesc);
		}

		m_assetList[ i ] = Asset(mesh, material);
	}
}

void AssetManager::LoadDebugSphereMaterial()
{
	SimpleDesc desc;
	desc.m_state.m_cullMode = Graphics::CullMode::NONE;
	desc.m_state.m_fillMode = Graphics::FillMode::WIREFRAME;
	Colour::Lightcyan.as_float_rgba_srgb( &desc.m_constants.diffuseColour.x );
	m_simpleSphereMaterial = Resources::CreateAsset< Material >(desc);
}

void AssetManager::LoadParticles()
{
	for ( int i = 0; i < static_cast< int >(ParticleType::TOTAL_PARTICLE_TYPES); i++ )
	{
		ComplexDesc desc;
		CreateParticleAsset( desc, static_cast<ParticleType>(i), "ShadowCast", g_vShaderPaths[static_cast<int>(ShaderType::PARTICLE_SHADER)]);
		m_particleList[ i ] = Resources::CreateAsset< Material >(desc);
	}

	ComplexDesc desc;
	CreateParticleAsset( desc, ParticleType::TYPE_STAR, "TwinklyStar", g_vShaderPaths[static_cast<int>(ShaderType::TWINKLY_STAR)]);
	m_twinklyStarMaterial = Resources::CreateAsset< Material >(desc);
}

void AssetManager::CreateParticleAsset( ComplexDesc& desc, ParticleType type, const char* name, const char* shaderPath )
{
	desc.SetupFromHLSLFile( name, shaderPath );
	desc.m_state.m_cullMode = Graphics::CullMode::NONE;
	desc.m_state.m_fillMode = Graphics::FillMode::SOLID;
	desc.m_state.m_blendEnable = true;
	desc.m_state.m_blendMode = Graphics::BlendMode::ADDITIVE;
	desc.m_state.m_depthEnable = true;
	desc.m_state.m_depthWrite = false;
	desc.m_texture[ 0 ] = Graphics::CreateTextureFromFile(g_vParticleSpritePaths[ static_cast< int >(type) ]);
	desc.m_sampler[ 0 ] = Graphics::CreateLinearSampler();
}
