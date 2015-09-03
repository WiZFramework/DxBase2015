//--------------------------------------------------------------------------------------
// File: PSSimple3D.hlsl
//
//--------------------------------------------------------------------------------------
#include "PRSimple3D.hlsli"


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
	//デフィーズ色とライト色を合成
	return input.Diffuse * LightColor;
}

