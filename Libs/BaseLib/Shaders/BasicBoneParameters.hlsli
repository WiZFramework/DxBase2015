
Texture2D<float4> Texture : register(t0);
// �[�x�}�b�v
Texture2D<float4> g_DepthMap : register(t1);

sampler Sampler : register(s0);
sampler g_SamplerDepthMap : register(s1);


cbuffer Parameters : register(b0)
{
	float4 DiffuseColor             : packoffset(c0);
	float3 EmissiveColor            : packoffset(c1);
	float3 SpecularColor            : packoffset(c2);
	float  SpecularPower : packoffset(c2.w);

	float3 LightDirection[3]        : packoffset(c3);
	float3 LightDiffuseColor[3]     : packoffset(c6);
	float3 LightSpecularColor[3]    : packoffset(c9);

	float3 EyePosition              : packoffset(c12);

	float3 FogColor                 : packoffset(c13);
	float4 FogVector                : packoffset(c14);

	float4x4 World                  : packoffset(c15);
	float3x3 WorldInverseTranspose  : packoffset(c19);
	float4x4 WorldViewProj          : packoffset(c22);

	float4x3 Bones[72]              : packoffset(c26);
};

cbuffer ShadowParameters : register(b1)
{
	uint4 activeflags				: packoffset(c0);	//�t���O�Ashadow = y
};

