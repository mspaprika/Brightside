#pragma once

class Player;



class AssetManager
{
	AssetManager();
	~AssetManager();

	PLAY_SINGLETON_INTERFACE(AssetManager);

public:

	using MeshId = Graphics::MeshId;
	using MaterialId = Graphics::MaterialId;
	using PbrDesc = Graphics::PbrMaterialDesc;
	using ComplexDesc = Graphics::ComplexMaterialDesc;
	using SimpleDesc = Graphics::SimpleMaterialDesc;
	using Material = Graphics::Material;

	struct Asset
	{
		MeshId mesh;
		MaterialId mat;

		Asset() {}
		Asset( MeshId meshID, MaterialId matID ) : mesh( meshID ), mat( matID ) {}
		Asset( const Asset&& a ) : mesh( a.mesh ), mat( a.mat ) {}

		Asset& operator = ( const Asset& a ) = default;
	};


	void LoadGameAssets();
	const Asset& GetAsset( AssetType type ) const { return m_assetList[ static_cast< int >(type) ]; }
	const MaterialId& GetParticleMaterial( ParticleType type ) const { return m_particleList[ static_cast< int >(type) ]; }
	const MaterialId& GetSphereMaterial() const { return m_simpleSphereMaterial; }
	const MaterialId& GetTwinklyStarMaterial() const { return m_twinklyStarMaterial; }

private:
	void LoadParticles();
	void LoadDebugSphereMaterial();
	void CreateParticleAsset( ComplexDesc& mat, ParticleType type, const char* name, const char* shaderPath );

	Asset m_assetList[ static_cast< int >(AssetType::TOTAL_ASSETS) ];
	MaterialId m_particleList[ static_cast< int >(ParticleType::TOTAL_PARTICLE_TYPES) ];
	MaterialId m_simpleSphereMaterial;
	MaterialId m_twinklyStarMaterial;
};
