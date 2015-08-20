//--------------------------------------------------------------------------------------
// File: PRSimple.hlsli
//
//--------------------------------------------------------------------------------------

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 model		: packoffset(c0);
	float4x4 view		: packoffset(c4);
	float4x4 projection	: packoffset(c8);
	float4 lightDirection : packoffset(c12);
	float4 diffuseColor	: packoffset(c13);
	float4x4 LightWorldViewProj	: packoffset(c14);   // ライトビューの正射影行列
	uint4 activeflags	: packoffset(c18);			//フラグ、texture=x,shadow = y,lightsize=z
	float4 LPos	: packoffset(c19);
	float4x4 lView	: packoffset(c20);
	float4x4 lProjection	: packoffset(c24);
	float4 EyePos	: packoffset(c28);
};


struct VertexShaderInput
{
	float4 Position : SV_Position;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct PixelShaderInputShadow
{
	float4 Position : SV_Position;
	float3 Normal   : NORMAL0;
	float3 lRay		: NORMAL1;
	float3 View : NORMAL2;
	float2 TexCoord : TEXCOORD0;
	float4 posLWVP  : POSITION1;
	float4 lightSpacePos : POSITION2;
};
