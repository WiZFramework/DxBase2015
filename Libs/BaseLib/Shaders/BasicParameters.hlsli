
//BasicParameters.hlsli


cbuffer Parameters : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);
    float3 EmissiveColor            : packoffset(c1);
    float3 SpecularColor            : packoffset(c2);
    float  SpecularPower            : packoffset(c2.w);

    float3 LightDirection[3]        : packoffset(c3);
    float3 LightDiffuseColor[3]     : packoffset(c6);
    float3 LightSpecularColor[3]    : packoffset(c9);

    float3 EyePosition              : packoffset(c12);

    float3 FogColor                 : packoffset(c13);
    float4 FogVector                : packoffset(c14);

    float4x4 World                  : packoffset(c15);
    float3x3 WorldInverseTranspose  : packoffset(c19);
    float4x4 WorldViewProj          : packoffset(c22);
	float4x4 LightWorldViewProj		: packoffset(c26);   // ライトビューの正射影行列
	uint4 activeflags				: packoffset(c30);	//フラグ、texture=x,shadow = y,lightsize=z,lightなし(テクスチャオンリー)=w

	float4 LPos: packoffset(c31);
	float4x4 lView: packoffset(c32);
	float4x4 lProjection: packoffset(c36);
	float4 EyePos: packoffset(c40);
};
