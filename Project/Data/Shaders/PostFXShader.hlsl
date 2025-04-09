#include "HLSL_StandardMacros"

#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 10
#define FXAA_GREEN_AS_LUMA 1

#include "FXAA3_11.hlsli"

// Pixel Shader Input
// This can be completely customized as long as it matches the output of your Vertex Shader.
struct PSInput
{
	noperspective float4 position : SV_POSITION; //<< This attribute must exist.
	noperspective float2 uv : UV;
};

Texture2D BaseOutputTex : register(PLAY3D_REG_GLOBAL_SRV0); // Finished Render

SamplerState RTSampler : register(PLAY3D_REG_LINEAR_CLAMP_SAMPLER); // Default Linear-Clamp sampler

// Post Processing constant buffer;
cbuffer PostProcessingConstantData : register(PLAY3D_REG_GLOBAL_CBV0)
{
	float4 RcpFrame; // XY frame size, Z is FXAA on/off
};

// The Vertex Shaders entry point function
PSInput VS_Main(uint id : SV_VertexID)
{
	PSInput output;
	// From https://www.nickiebba.com/single-post/2017/08/23/post-processing-the-triangle-trick
	// Calculate the UV via the id
	output.uv = float2((id << 1) & 2, id & 2);

	output.position = float4(output.uv, 0, 1);
	output.position.x = output.position.x * 2 - 1;
	output.position.y = output.position.y * -2 + 1;

	return output;
}

float4 PS_Main(PSInput input) : SV_TARGET
{
	float3 buffer = BaseOutputTex.Sample(RTSampler, input.uv);

	FxaaTex InputFXAATex = { RTSampler, BaseOutputTex };

	half4 FXAA = FxaaPixelShader
	   (input.uv,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),
		InputFXAATex,
		InputFXAATex,
		InputFXAATex,
		RcpFrame.xy,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),
		1.00f,
		0.063f,
		0.0312f,
		0.0f,
		0.0f,
		0.0f,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f));

	if (RcpFrame.z > 0.5f)
	{
		return FXAA;
	}

	return float4(buffer, 1.0f);
}