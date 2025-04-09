///////////////////////////////////////////////////////////////////////////
//	File		: Main.cpp
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////
#include "GameMath.h"
#include "Types.h"
#include "GameStateManager.h"
#include "RenderContext.h"
#include "Cameras/CameraManager.h"
#include "ObjectFactory/GameObjectManager.h"


constexpr u32 kShadowMapDim = 4096;
using namespace Play3d;

struct PbrData
{
	float params[4];
	float dummy0[4];
	float dummy1[4];
	float dummy2[4];
};

void LoadSkyAndIBL()
{
	static constexpr u32 kEnvMapCount = 3;
	static const char* EnvMapPaths[kEnvMapCount] = {
		"Data/Skybox/space_sky.dds",
		"Data/Skybox/space_rad.dds",
		"Data/Skybox/space_irr.dds",
	};

	Graphics::TextureId EnvMaps[kEnvMapCount];
	
	for (size_t i = 0; i < kEnvMapCount; i++)
	{
		EnvMaps[i] = Graphics::CreateTextureFromDDS(EnvMapPaths[i]);
	}

	Graphics::SetEnvironmentMaps(EnvMaps[0], EnvMaps[1], EnvMaps[2]);
}

class GameAppClass
{
	RenderContext m_renderContext;

	Graphics::BufferId m_postfxConstantBufferId;
	Graphics::BufferId m_blackHoleConstantBufferId;

	struct PostProcessingConstantData
	{
		Vector4f RcpFrame; // Constants for the FXAA shader.
	};

	// this is my attempt to generate Game Over PostFX - didn't work!
	struct GameOverConstantData
	{
		Vector4f RcpFrame;
		Vector2f ScreenResolution;
		Vector2f Time;
	};
	GameOverConstantData m_gameOverConstantData = {};
	Graphics::MaterialId m_gameOverMaterialId;

	PostProcessingConstantData m_postfxConstantData = {};
	Graphics::MaterialId m_postfxMaterialId;

	UI::FontId m_debugFontId;

	Graphics::TextureId m_shadowMapDepthTargetArray;
	Graphics::SamplerId m_shadowMapSampler;

public:
	void Initialise()
	{
		// First we initialize the Play3d library.
		SystemDesc systemDesc;
		systemDesc.title = "Bright Purple Side";
		systemDesc.width = 1920;
		systemDesc.height = 1024;
		System::Initialise(systemDesc);

		// Grab the debug font.
		m_debugFontId = UI::GetDebugFont();

		///////////////////////////////////////////////////////// Initialize Singletons ////////////////////////////////////////////////////////////
		GameStateManager::Initialise();
		

		// Load the skybox
		LoadSkyAndIBL();

		// Shadow map array
		{
			Graphics::TextureDesc desc;
			desc.type = Graphics::TextureType::TEXTURE2D_ARRAY;
			desc.format = Graphics::TextureFormat::DEPTH;
			desc.height = kShadowMapDim;
			desc.width = kShadowMapDim;
			desc.slices = kNumLights;
			desc.mipLevels = 1;
			desc.flags = Graphics::TextureFlags::ENABLE_RENDER_TARGET | Graphics::TextureFlags::ENABLE_TEXTURE;
			desc.pDebugName = "Shadow Map Array";

			m_shadowMapDepthTargetArray = Resources::CreateAsset<Graphics::Texture>(desc);
		}

		// Shadow Map sampler
		{
			Graphics::SamplerDesc desc;
			desc.m_filter = Graphics::FilterMode::BILINEAR;
			desc.SetUniformAddressMode(Graphics::AddressMode::CLAMP);
			m_shadowMapSampler = Resources::CreateAsset<Graphics::Sampler>(desc);
		}

		// Shadow Casting material
		{
			Graphics::ComplexMaterialDesc desc;
			desc.SetupFromHLSLFile("ShadowCast", "Data/Shaders/ShadowCastShader.hlsl");
			desc.m_state.m_cullMode = Graphics::CullMode::FRONT;
			desc.m_state.m_fillMode = Graphics::FillMode::SOLID;
			desc.m_bNullPixelShader = true;
			m_shadowCastMatId = Resources::CreateAsset<Graphics::Material>(desc);
		}

		// Define PostFX material
		{
			Graphics::ComplexMaterialDesc desc;
			desc.SetupFromHLSLFile("ShadowCast", "Data/Shaders/PostFXShader.hlsl");
			desc.m_state.m_cullMode = Graphics::CullMode::NONE;
			desc.m_state.m_fillMode = Graphics::FillMode::SOLID;
			desc.m_state.m_depthEnable = false;
			desc.m_state.m_depthWrite = false;
			m_PostFXMatId = Resources::CreateAsset<Graphics::Material>(desc);
		}

		//  GameOver PostFX material
		{
			Graphics::ComplexMaterialDesc desc;
			desc.SetupFromHLSLFile("Black Hole", "Data/Shaders/GameOver.hlsl");
			desc.m_state.m_cullMode = Graphics::CullMode::NONE;
			desc.m_state.m_fillMode = Graphics::FillMode::SOLID;
			desc.m_state.m_depthEnable = false;
			desc.m_state.m_depthWrite = false;
			m_gameOverMaterialId = Resources::CreateAsset<Graphics::Material>(desc);
		}
		 // create GameOver PostFX constant buffer
		{
			Graphics::BufferDesc desc;
			desc.m_bindFlags = Graphics::BufferBindFlags::CONSTANT;
			desc.SetConstantBuffer<GameOverConstantData>(&m_gameOverConstantData);
			m_blackHoleConstantBufferId = Resources::CreateAsset<Graphics::Buffer>(desc);
		}

		// Create post fx constant buffer
		{
			Graphics::BufferDesc desc;
			desc.m_bindFlags = Graphics::BufferBindFlags::CONSTANT;
			desc.SetConstantBuffer<PostProcessingConstantData>(&m_postfxConstantData);
			m_postfxConstantBufferId = Resources::CreateAsset<Graphics::Buffer>(desc);
		}

		//////////////////////////////////////////////////// Generate Level 1 //////////////////////////////////////////////////////////////
		// will be moved into update once the game states are sorted
		GameStateManager::Instance().GenerateLevel();
	}

	bool Update()
	{
		bool bKeepGoing = true;
		// BeginFrame should be called first, it will return RESULT_QUIT if the user has quit via 'Close' icons.
		if (System::BeginFrame() != RESULT_OK)
		{
			bKeepGoing = false;
		}

		// We want to quit on the next frame if the user presses ESC.
		if (Input::IsKeyPressed(VK_ESCAPE))
		{
			bKeepGoing = false;
		}

		GameStateManager::Instance().Update( System::GetDeltaTime() );
		
		return bKeepGoing;
	}

	void Render()
	{
		// Pass 1: Shadowmap Rendering
		Graphics::PushMarker("Shadowmap Rendering");
		m_renderContext.BeginPass(RenderPassType::kShadowPass);

		// Render any objects that should be in the shadow pass here. 
		// Your game objects will get the context, so can enquire what pass is being rendered.
		GameObjectManager::Instance().Render(m_renderContext);

		m_renderContext.EndPass();
		// Render shadow objects to shadow buffer.

		for (u16 sliceIndex = 0; sliceIndex < kNumLights; sliceIndex++)
		{
			Graphics::LightType type = Graphics::GetLightType(sliceIndex);
			if (type == Graphics::LightType::LIGHT_POINT)
				continue; // We do NOT support point light shadows. No way.

			Matrix4x4f lightViewMatrix = Graphics::CreateShadowLightViewMatrix(sliceIndex);

			Matrix4x4f lightProjectMatrix;
			if (type == Graphics::LightType::LIGHT_DIRECTIONAL)
				lightProjectMatrix = Graphics::CreateShadowLightProjectMatrix(sliceIndex, 2.0f, -32, 32);
			else
				lightProjectMatrix = Graphics::CreateShadowLightProjectMatrix(sliceIndex, 0.0f, 0.1f, 100.f);

			Graphics::SetViewport(Graphics::Viewport({ kShadowMapDim,kShadowMapDim })); // prob scissor too.
			Graphics::SetViewMatrix(lightViewMatrix);
			Graphics::SetProjectionMatrix(lightProjectMatrix);

			// Clear the shadow map and Render the Shadow Casting Objects	
			Graphics::ClearDepthTarget(m_shadowMapDepthTargetArray, 1.0f, 0, sliceIndex);
			Graphics::SetDepthOnlyTarget(m_shadowMapDepthTargetArray, 0, sliceIndex);

			Graphics::SetLightMatrix(sliceIndex, lightProjectMatrix * lightViewMatrix);

			// Colour is not important to us for the shadow map.
			// We only want to render to the Depth buffer.
			Graphics::SetMaterial(m_shadowCastMatId);
			for (const MeshRenderCommand& cmd : m_renderContext.GetMeshCommands())
			{
				Graphics::DrawMesh(cmd.meshId, cmd.worldMatrix);
			}
		}
		Graphics::PopMarker();

		Graphics::PushMarker("Main Render Pass");
		// Get Transient Texture for PostFX
		Graphics::TextureId postFXSurf = GetTransientSurface(Graphics::GetDisplaySurfaceSize(), Graphics::TextureFormat::RGBA_SRGB);
		Graphics::TextureId postFXDepthSurf = GetTransientSurface(Graphics::GetDisplaySurfaceSize(), Graphics::TextureFormat::DEPTH);

		Graphics::ClearRenderTarget(postFXSurf, Colour::Black);
		Graphics::ClearDepthTarget(postFXDepthSurf);

		// Pass 2: Forward rendering
		m_renderContext.BeginPass(RenderPassType::kForwardPass);

		GameObjectManager::Instance().Render(m_renderContext);

		m_renderContext.EndPass();

		Graphics::SetRenderTargets(postFXSurf, postFXDepthSurf);
		Graphics::SurfaceSize surfaceSize = Graphics::GetDisplaySurfaceSize();
		Graphics::SetViewport(Graphics::Viewport(surfaceSize));
		CameraManager::Instance().ApplyActiveCameraMatrices();

		Graphics::BindGlobalTexture(Graphics::kShadowMapTextureSlot, m_shadowMapDepthTargetArray, m_shadowMapSampler, Graphics::ShaderStageFlag::PIXEL_STAGE, Graphics::TextureBindFlags::SRV);

		// Render all the meshes
		for (const MeshRenderCommand& cmd : m_renderContext.GetMeshCommands())
		{
			Graphics::SetMaterial(cmd.materialId);
			Graphics::DrawMesh(cmd.meshId, cmd.worldMatrix);
		}

		Graphics::DrawEnvironmentSkybox();

		// Render all the particle FX
		for (const ParticleRenderCommand& cmd : m_renderContext.GetParticleCommands())
		{
			Graphics::SetMaterial(cmd.materialId);
			Graphics::SetWorldMatrix(cmd.worldMatrix);
			Graphics::BindGlobalBuffer(Graphics::kGlobalBufferSlotStart, cmd.particleBufferId, Graphics::ShaderStageFlag::VERTEX_STAGE, Graphics::BufferBindFlags::SRV);
			Graphics::DrawWithoutVertices(cmd.particleCount * 6);
		}

		Graphics::PopMarker();

		// Pass 3: PostFX Rendering
		Graphics::PushMarker("Post Processing Effects Rendering");
		Graphics::SetRenderTargetsToSwapChain(true);

		m_postfxConstantData.RcpFrame = Vector4f(1.0f / Graphics::GetDisplaySurfaceSize().m_width, 1.0f / Graphics::GetDisplaySurfaceSize().m_height, true, 0.0f);
		Graphics::UpdateBuffer(m_postfxConstantBufferId, &m_postfxConstantData, sizeof(m_postfxConstantData));
		Graphics::BindGlobalBuffer(Graphics::kGlobalBufferSlotStart, m_postfxConstantBufferId, Graphics::ShaderStageFlag::PIXEL_STAGE);
		Graphics::BindGlobalTexture(Graphics::kGlobalTextureSlotStart, postFXSurf, Graphics::SamplerId(), Graphics::ShaderStageFlag::PIXEL_STAGE, Graphics::TextureBindFlags::SRV);

		Graphics::SetMaterial(m_PostFXMatId);
		Graphics::DrawWithoutVertices(3);

		Graphics::BindGlobalTexture(Graphics::kGlobalTextureSlotStart, Graphics::TextureId(), Graphics::SamplerId(), Graphics::ShaderStageFlag::PIXEL_STAGE, Graphics::TextureBindFlags::SRV);
		Graphics::PopMarker();

		// Pass 4: Debug Rendering ///////////////////////////////////// DEBUG PASS ///////////////////////////////////////////////////////////////
		Graphics::PushMarker("Debug Rendering");
		m_renderContext.BeginPass( RenderPassType::kDebugPass );
		GameObjectManager::Instance().Render( m_renderContext );
		m_renderContext.EndPass();

		////////////////////////////////// Draw Debugging axes for player and light ////////////////////////////////////////////////////////////////
		Graphics::BeginPrimitiveBatch(false);
		//GameStateManager::Instance().DrawDebugAxes();

		for (const AxisRenderCommand& cmd : m_renderContext.GetAxisCommands())
		{
			//Graphics::DrawAxes( cmd.worldMatrix, 1.0f );
		}

		Graphics::EndPrimitiveBatch();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GameStateManager::Instance().DrawUI();
		//UI::DrawString(m_debugFontId, Vector2f(20, 20), Colour::White, "Skills Project 2 Starter");
		UI::DrawPrintf(m_debugFontId,
			Vector2f(20, 40),
			Colour::Lightblue,
			"[delta=%.2fms elapsed=%.2fs gpu=%.2fms]",
			System::GetDeltaTime() * 1000.f,
			System::GetElapsedTime(),
			Graphics::GetGPUFrameTime());
		// End frame
		System::EndFrame();
		Graphics::PopMarker();
	}

	void Shutdown()
	{
		// Destroy our shadowmaps textures.

		// Make sure to shutdown the library before we end our main function.
		System::Shutdown();

	}
	Graphics::MaterialId m_shadowCastMatId;
	Graphics::MaterialId m_PostFXMatId;
};

int PlayMain()
{
	GameAppClass app;

	app.Initialise();
	while (app.Update())
	{	
		app.Render();
	}

	app.Shutdown();

	return 0;
};