#include "HLSL_FrameConstants"
#include "HLSL_DrawConstants"
#include "HLSL_MeshInputs"
#include "HLSL_LightConstants"


cbuffer MaterialConstantData : register(PLAY3D_REG_MATERIAL_CONSTANTS)
{
    float4 materialParams;
};

// Pixel Shader Input
// This can be completely customized as long as it matches the output of your Vertex Shader.
struct PSInput
{
// sceen space
    float4 position : SV_POSITION; //<< This attribute must exist.
    float4 colour : COLOUR;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 pos_w : BOB;
};

Texture2D myTexture : register(t0);
Texture2D myTexture2 : register(t1);
Texture2D myTexture3 : register(t2);
SamplerState myState : register(s0);

// The Vertex Shaders entry point function
// This is called on every vertex in the mesh.
PSInput VS_Main(VSInput input)
{
    PSInput output;

    output.position = mul(mvpMtx, float4(input.position.xyz, 1.0f));
    output.colour = pow(input.colour, 2.2); // Gamma correct from sRGB.
    output.normal = mul(normalMtx, float4(input.normal.xyz, 0.0f));
    output.uv = input.uv;
    output.pos_w = mul(worldMtx, float4(input.position.xyz, 1.0f));

    return output;
}

// The Pixel Shaders entry point function
// This is called on every generated pixel (fragment).
float4 PS_Main(PSInput input) : SV_TARGET
{
    float4 tc = myTexture.Sample(myState, input.uv);
	//return float4(tc.xyz, 1);
    return float4(input.uv.xy, 1, 1);
}