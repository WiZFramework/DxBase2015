//--------------------------------------------------------------------------------------
// File: PSTexture3D.hlsl
//
//--------------------------------------------------------------------------------------
#include "PRTexture3D.hlsli"

Texture2D<float4> SimpleTexture : register(t0);
SamplerState SimpleSampler : register(s0);


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PixelShaderInput input) : SV_TARGET
{
	//ライトの向きを得る
	float3 LightDirection = normalize(float3(LightDir.xyz));
	float3 InputNormal = normalize(input.Normal);
	//ライトによるピクセルの色を決定
	float4 LightColor = saturate(dot(InputNormal, LightDirection) + float4(0.7, 0.7, 0.7, 1.0));
	//ライトの透明処理はなし
	LightColor.a = 1;
	//テクスチャと合わせる
	float4 TextureColor = SimpleTexture.Sample(SimpleSampler, input.TexCoord);
	//テクスチャ色とライト色を合成
	return TextureColor *LightColor;
}

