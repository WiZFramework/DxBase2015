//--------------------------------------------------------------------------------------
// File: PSSimplePNT.hlsl
//
//--------------------------------------------------------------------------------------
#include "PRSimple.hlsli"

Texture2D<float4> SimpleTexture : register(t0);
// 深度マップ
Texture2D g_DepthMap : register(t1);
SamplerState SimpleSampler : register(s0);

SamplerState g_SamplerDepthMap : register(s1);

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PixelShaderInputShadow input) : SV_TARGET
{
	const float3 ambient = float3(0.1f, 0.1f, 0.1f);

	float3 shadowColor = float3(1.0f, 1.0f, 1.0f);
	// テクセルを計算
	float2 texel = float2(input.posLWVP.x / input.posLWVP.w * 0.5f + 0.5f, input.posLWVP.y / input.posLWVP.w * -0.5f + 0.5f);
	float depth = input.posLWVP.z / input.posLWVP.w - 0.00005f;
	float sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
	if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
		// 現在の深度値と深度マップ上の深度値を比較
		if (sm < depth){
			shadowColor.xyz = 0.6f;
		}
		else{
			shadowColor.xyz = 1.0f;
		}
	}

	float3 lightdir = normalize(lightDirection.xyz);
	float3 N1 = normalize(input.Normal);
	float4 lightcolor = (0.6f * saturate(dot(N1, -lightdir)) + 0.4f);
	float4 Diffuse = diffuseColor + lightcolor;
	//テクスチャと合わせる
	if (activeflags.x == 1){
		//テクスチャと合わせる
		Diffuse = SimpleTexture.Sample(SimpleSampler, input.TexCoord) * Diffuse;
	}
	Diffuse.rgb = Diffuse.rgb * shadowColor;
	return Diffuse;

}

