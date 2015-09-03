//--------------------------------------------------------------------------------------
// File: PRSimple3D.hlsli
//
//--------------------------------------------------------------------------------------

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 Model		: packoffset(c0);
	float4x4 View		: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir : packoffset(c12);
};


struct VertexShaderInput
{
	float4 Position : SV_Position;
	float3 Normal   : NORMAL;
	float4 Diffuse    : COLOR0;
};

struct PixelShaderInput
{
	float4 Position : SV_Position;
	float3 Normal   : NORMAL;
	float4 Diffuse    : COLOR0;
};
