#include "HLSL_FrameConstants"
#include "HLSL_DrawConstants"

// Vertex Shader Input
struct VSInput
{
	float3 position : POSITION;
};

// Pixel Shader Input
struct PSInput
{
	float4 position : SV_POSITION; //<< This attribute must exist.
};

// The Vertex Shaders entry point function
PSInput VS_Main(VSInput input)
{
	PSInput output;
	output.position = mul(mvpMtx, float4(input.position.xyz, 1.0f));
	return output;
}

// The Pixel Shaders entry point function
float4 PS_Main(PSInput input) : SV_TARGET
{
	return float4(1,1,1,1);
}