///////////////////////////////////////////////////////////////////////////
//	File		: RenderContext.cpp
//	Platform	: All
//
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include "GameMath.h"
#include "RenderContext.h"


RenderContext::RenderContext()
{
	m_meshRenderCommands.reserve(256);
	m_particleCommands.reserve(256);
}

void RenderContext::BeginPass( RenderPassType type )
{
	m_passType = type;
	m_meshRenderCommands.clear();
	m_particleCommands.clear();
	m_axisCommands.clear();
}

RenderPassType RenderContext::GetRenderPassType() const
{
	return m_passType;
}

void RenderContext::RenderMesh( const MeshRenderCommand& cmd )
{
	m_meshRenderCommands.push_back( cmd );
}

void RenderContext::RenderParticles( const ParticleRenderCommand& cmd )
{
	m_particleCommands.push_back( cmd );
}

void RenderContext::RenderAxis( const AxisRenderCommand& cmd )
{
	m_axisCommands.push_back(cmd);
}

void RenderContext::EndPass()
{

}
