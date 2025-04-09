#include "HLSL_FrameConstants"
#include "HLSL_DrawConstants"

// Material textures.
Texture2D g_alphaTexture : register(t0);
SamplerState g_alphaSampler : register(s0);

// Vertex Shader Input
// In this example we generate the vertices in the vertex shader.
struct VSInput
{
    uint vertexId : SV_VertexID;
};

// We can pull in particle data via a structured buffer.
// Particle should match the version on the CPU.
struct Particle
{
    float scale;
    float3 pos;
    float3 vel;
    float3 colour;
    float4 life; // extend
};

StructuredBuffer<Particle> g_particles : register(t8);

// Pixel Shader Input
// This can be completely customized as long as it matches the output of your Vertex Shader.
struct PSInput
{
    float4 position : SV_POSITION; //<< This attribute must exist.
    float2 uv : TEXCOORD;
    float4 colour : COLOUR;
};

static const float3 g_quadOffset[6] =
{
    float3(-1, -1, 0),
	float3(1, -1, 0),
	float3(1, 1, 0),

	float3(-1, -1, 0),
	float3(1, 1, 0),
	float3(-1, 1, 0)
};

static const float2 g_quadUV[6] =
{
    float2(0, 0),
	float2(1, 0),
	float2(1, 1),

	float2(0, 0),
	float2(1, 1),
	float2(0, 1)
};

PSInput VS_Main(VSInput input)
{
    uint vid = input.vertexId % 6; // Quad Vertex ID
    uint pid = input.vertexId / 6; // Particle ID

	// Grab Particle Data
    float3 position = g_particles[pid].pos;
    float3 velocity = g_particles[pid].vel;
    float lifeTime = g_particles[pid].life[0];
    float3 colour = g_particles[pid].colour;
    float scale = g_particles[pid].scale;

    PSInput output;

	// Extract world space view for billboard orientation.
    float3 camRight = -viewMtx[0];
    float3 camUp = viewMtx[1];

    float fSize = (length(position) * length(position) * 0.0005f) + scale + sin(lifeTime * (5.0f + (3.0F * 5.0f * velocity.y))) * (0.1f + (0.1f * velocity.y));

    float3 quadOffset = (g_quadOffset[vid].x * camRight + g_quadOffset[vid].y * camUp) * fSize;
	
    output.uv = g_quadUV[vid];

    output.position = mul(viewProjectionMtx, float4(position.xyz + quadOffset, 1.0));
    output.colour = float4(colour, 1.0f);
    return output;
}

// Pixel Shader
float4 PS_Main(PSInput input) : SV_TARGET
{
    float fA = g_alphaTexture.Sample(g_alphaSampler, input.uv).r;
	
    return float4(input.colour.xyz, fA);
}