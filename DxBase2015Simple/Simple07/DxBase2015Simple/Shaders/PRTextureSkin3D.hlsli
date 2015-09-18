//--------------------------------------------------------------------------------------
// File: PRTexture3D.hlsli
//
//--------------------------------------------------------------------------------------

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 Model		: packoffset(c0);
	float4x4 View		: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir : packoffset(c12);
	float4x3 Bones[72] : packoffset(c13);
};


struct VertexShaderInput
{
	float4 Position : SV_Position;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD0;
	uint4  Indices  : BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};

struct PixelShaderInput
{
	float4 Position : SV_Position;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD0;
};
