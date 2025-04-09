


const char* g_vParticleSpritePaths[ static_cast< int >(ParticleType::TOTAL_PARTICLE_TYPES) ]
{
	"Data/ParticleSprites/star_01.png",
	"Data/ParticleSprites/dirt_01.png",
	"Data/ParticleSprites/scorch_01.png",
	"Data/ParticleSprites/pixel.png",
	"Data/ParticleSprites/spark.png",
	"Data/ParticleSprites/explosion.png",
	"Data/ParticleSprites/magic_05.png",
	"Data/ParticleSprites/flare.png",
	"Data/ParticleSprites/snow.png",
	"Data/ParticleSprites/fluid.png",
};

const char* g_vAssetMeshPaths[ static_cast< int >(AssetType::TOTAL_ASSETS) ]
{
	"Data/Player/updated_ship_0.p3dmesh",
	"Data/Planet/Planet1_0.p3dmesh",
	"Data/UFO/UFO_0.p3dmesh",
	"Data/Missiles/Spark/Spark_001_0.p3dmesh",
	"Data/Missiles/Missile_01_0.p3dmesh",
	"Data/Volcanos/volcano_fixed_0.p3dmesh",
	"Data/Volcanos/volcano_fixed2_0.p3dmesh",
	"Data/Powers/powerup_health_0.p3dmesh",
	"Data/Powers/powerup_shield_0.p3dmesh",
	"Data/Powers/powerup_moveSpeed_0.p3dmesh",
	"Data/Asteroids/small/Planet_Sphere_004_0.p3dmesh",
	"Data/Asteroids/medium/Planet_Sphere_004_0.p3dmesh",
	"Data/Asteroids/large/Planet_Sphere_004_0.p3dmesh",
	"Data/Debris/Debris_Icosphere_004_small.p3dmesh",
	"Data/Debris/Debris_Icosphere_004_med.p3dmesh",

};

const char* g_vAssetColourPaths[ static_cast< int >(AssetType::TOTAL_ASSETS) ] // Albedo (colour)
{
	"Data/Player/Steel_Blue_Albedo.png",
	"Data/Planet/Planet_basecolor.png",
	"Data/UFO/UFO_UFO_BaseColor.png",
	"Data/Missiles/Spark/Sparkles_Sparkles_BaseColor.png",
	"Data/Missiles/Missile_Missile_BaseColor.png",
	"Data/Volcanos/VolcanoesNCraters_CraterNVolcanoes_BaseColor.png",
	"Data/Volcanos/VolcanoesNCraters_CraterNVolcanoes_BaseColor.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_BaseColor.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_BaseColor.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_BaseColor.png",
	"Data/Debris/PlanetSurface_003_basecolor_2k.png",
	"Data/Debris/PlanetSurface_003_basecolor_2k.png",
	"Data/Debris/PlanetSurface_003_basecolor_2k.png",
	"Data/Debris/PlanetSurface_003_basecolor_2k.png",
	"Data/Debris/PlanetSurface_003_basecolor_2k.png",

};

const char* g_vAssetNormalPaths[ static_cast< int >(AssetType::TOTAL_ASSETS) ] // Normal map
{
	"Data/Player/Painted_Steel_normal.png",
	"Data/Planet/Planet_normal.png",
	"Data/UFO/UFO_UFO_Normal.png",
	"Data/Missiles/Spark/Sparkles_Sparkles_Normal.png",
	"Data/Missiles/Missile_Missile_Normal.png",
	"Data/Volcanos/VolcanoesNCraters_CraterNVolcanoes_Normal.png",
	"Data/Volcanos/VolcanoesNCraters_CraterNVolcanoes_Normal.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_Normal.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_Normal.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_Normal.png",
	"Data/Debris/PlanetSurface_003_normal_2k.png",
	"Data/Debris/PlanetSurface_003_normal_2k.png",
	"Data/Debris/PlanetSurface_003_normal_2k.png",
	"Data/Debris/PlanetSurface_003_normal_2k.png",
	"Data/Debris/PlanetSurface_003_normal_2k.png",

};

const char* g_vAssetORMPaths[ static_cast< int >(AssetType::TOTAL_ASSETS) ] // ORM (Ambient Occlusion, Roughness, Metalness)
{
	"Data/Player/Painted_Steel_ORM.png",
	"Data/Planet/Planet_AORM.png",
	"Data/UFO/UFO_UFO_OcclusionRoughnessMetallic.png",
	"Data/Missiles/Spark/Sparkles_Sparkles_OcclusionRoughnessMetallic.png",
	"Data/Missiles/Missile_Missile_OcclusionRoughnessMetallic.png",
	"Data/Volcanos/VolcanoesNCraters_CraterNVolcanoes_OcclusionRoughnessMetallic.png",
	"Data/Volcanos/VolcanoesNCraters_CraterNVolcanoes_OcclusionRoughnessMetallic.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_OcclusionRoughnessMetallic.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_OcclusionRoughnessMetallic.png",
	"Data/Powers/powerupMeshes_DefaultMaterial_OcclusionRoughnessMetallic.png",
	"Data/Debris/PlanetSurface_003_ORM_2k.png",
	"Data/Debris/PlanetSurface_003_ORM_2k.png",
	"Data/Debris/PlanetSurface_003_ORM_2k.png",
	"Data/Debris/PlanetSurface_003_ORM_2k.png",
	"Data/Debris/PlanetSurface_003_ORM_2k.png",

};

const char* g_vShaderPaths[ static_cast< int >(ShaderType::TOTAL_SHADERS) ]
{
	"Data/Shaders/ParticleShader.hlsl",
	"Data/Shaders/TwinklyStar.hlsl",
	"Data/Shaders/SelfIllumination.hlsl",
};