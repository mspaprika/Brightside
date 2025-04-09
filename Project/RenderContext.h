///////////////////////////////////////////////////////////////////////////
//	File		: RenderContext.h
//	Platform	: All
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

#pragma once
enum class RenderPassType
{
	kShadowPass,
	kZPrepass,
	kForwardPass,
	kDebugPass,
};

struct MeshRenderCommand
{
	Matrix4x4f worldMatrix;
	Graphics::MeshId meshId;
	Graphics::MaterialId materialId;
	u32 subMeshIndex = 0;
};

struct ParticleRenderCommand
{
	Matrix4x4f worldMatrix;
	Graphics::MaterialId materialId;
	Graphics::BufferId particleBufferId;
	u32 particleCount;
};

struct AxisRenderCommand
{
	Matrix4x4f worldMatrix;
};

// Render context collects information for a each render pass.
class RenderContext
{
public:
	using MeshCmdList = std::vector< MeshRenderCommand >;
	using ParticlesCmdList = std::vector< ParticleRenderCommand >;
	using AxisRenderCmdList= std::vector< AxisRenderCommand >;

	RenderContext();
	void BeginPass(RenderPassType type);
	RenderPassType GetRenderPassType() const;
	void RenderMesh( const MeshRenderCommand& cmd );
	void RenderParticles( const ParticleRenderCommand& cmd );
	void RenderAxis(const AxisRenderCommand& cmd);
	void EndPass();
	const MeshCmdList& GetMeshCommands() const { return m_meshRenderCommands; }
	const ParticlesCmdList& GetParticleCommands() const { return m_particleCommands; }
	const AxisRenderCmdList& GetAxisCommands() const { return m_axisCommands; }
private:
	RenderPassType m_passType;
	MeshCmdList m_meshRenderCommands;
	ParticlesCmdList m_particleCommands;
	AxisRenderCmdList m_axisCommands;
};
